#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLoggingCategory>

#include <jd-util/Exception.h>

class Transaction
{
	QSqlDatabase m_db;
	bool m_didCommit = false;
	static bool m_inTransaction;
public:
	explicit Transaction(QSqlDatabase db = QSqlDatabase::database());
	~Transaction();

	void commit();
};

class DatabaseException : public Exception
{
public:
	explicit DatabaseException(const QString &error, const QString &query = QString());
};

namespace DB {
QSqlQuery check(const QSqlQuery &query);
void check(const QSqlQuery &query, const bool success);

int defaultPort(const QString &driver);

QSqlDatabase fromUrl(const QUrl &url);
}

// need to call Q_LOGGING_CATEGORY(Database, "...") in application code
Q_DECLARE_LOGGING_CATEGORY(Database)
