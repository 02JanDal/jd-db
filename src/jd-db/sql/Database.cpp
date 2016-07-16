#include "Database.h"

#include <QSqlError>
#include <QUrl>
#include <QFileInfo>
#include <QDir>

#include <jd-util/Functional.h>

using namespace JD::Util;

static QString userToDbType(const QString &userType)
{
	if (userType == "postgres") {
		return "QPSQL";
	} else if (userType == "oracle") {
		return "QOCI";
	} else {
		return "Q" + userType.toUpper();
	}
}
static QString dbToUserType(const QString &dbType)
{
	if (dbType == "QPSQL") {
		return "postgres";
	} else if (dbType == "QOCI") {
		return "oracle";
	} else {
		return QString(dbType).remove(0, 1).toLower();
	}
}

bool Transaction::m_inTransaction = false;

Transaction::Transaction(QSqlDatabase db)
	: m_db(db)
{
	Q_ASSERT_X(!m_inTransaction, "Transaction::Transaction", "Already in a transaction");
	if (!m_db.transaction()) {
		throw DatabaseException("Unable to begin transaction: " + m_db.lastError().text());
	}
	m_inTransaction = true;
}
Transaction::~Transaction()
{
	m_inTransaction = false;
	if (!m_didCommit) {
		m_db.rollback();
	}
}
void Transaction::commit()
{
	if (!m_db.commit()) {
		throw DatabaseException("Unable to commit transaction: " + m_db.lastError().text());
	}
	m_didCommit = true;
}

DatabaseException::DatabaseException(const QString &error, const QString &query)
	: Exception(error)
{
	if (!query.isNull()) {
		qCWarning(Database) << "Failed query" << query << ":" << error;
	}
}

QSqlQuery DB::check(const QSqlQuery &query)
{
	if (query.lastError().isValid()) {
		throw DatabaseException(query.lastError().text(), query.lastQuery());
	}
	return query;
}
void DB::check(const QSqlQuery &query, const bool success)
{
	if (!success) {
		throw DatabaseException(query.lastError().text(), query.lastQuery());
	}
}

int DB::defaultPort(const QString &driver)
{
	if (driver == "QPSQL") {
		return 5432;
	} else if (driver == "QMYSQL") {
		return 3306;
	} else if (driver == "QSQLITE" || driver == "QSQLITE2") {
		return -1;
	} else {
		Q_ASSERT_X(false, "ServerDatabasePrivate::defaultPort", "need to add the default port for the given server driver");
	}
}

QSqlDatabase DB::fromUrl(const QUrl &url)
{
	const QString type = userToDbType(url.scheme());
	if (!QSqlDatabase::drivers().contains(type)) {
		throw DatabaseException(QString("Unknown database type '%1', available types: '%2'").arg(url.scheme(), Functional::map2<QStringList>(QSqlDatabase::drivers(), &dbToUserType).join("', '")));
	}
	QSqlDatabase db = QSqlDatabase::addDatabase(type);
	if (type.startsWith("QSQLITE")) {
		QUrl localUrl = url;
		localUrl.setScheme("file");
		QFileInfo file(localUrl.toLocalFile());
		file.absoluteDir().mkpath(file.absoluteDir().absolutePath());
		db.setDatabaseName(localUrl.toLocalFile());
	} else {
		db.setHostName(url.host());
		db.setPort(url.port(defaultPort(type)));
		db.setUserName(url.userName());
		db.setPassword(url.password());
		db.setDatabaseName(url.path());
	}
	db.setConnectOptions(url.query().split('&').join(';'));
	return db;
}
