#include "Dialects.h"

#include <QString>
#include <QVariant>
#include <QUuid>
#include <QSqlDatabase>
#include <QDate>
#include <QDateTime>
#include <QTime>

#include <jd-util/Compiler.h>

#include "Database.h"

namespace DB {

Dialect Dialect::fromDb(const QSqlDatabase &db)
{
	const QString driver = db.driverName();
	if (driver == "QPSQL") {
		return Dialect(Type::Postgres);
	} else if (driver == "QMYSQL") {
		return Dialect(Type::MySQL);
	} else if (driver == "QSQLITE" || driver == "QSQLITE2") {
		return Dialect(Type::Sqlite);
	} else {
		throw DatabaseException("Unhandled database type, don't know the dialect");
	}
}

Dialect::Dialect(const Dialect::Type type)
	: m_type(type) {}

QVariant Dialect::convertToDatabase(const QVariant &value) const
{
	if (value.type() == QVariant::Uuid) {
		if (m_type == Type::Postgres) {
			return value.toUuid().toString();
		} else {
			return value.toUuid().toRfc4122();
		}
	}
	return value;
}
QVariant Dialect::convertFromDatabase(const ColumnType type, const QVariant &value) const
{

	switch (type) {
	case ColumnType::Serial: return value.toInt();
	case ColumnType::Boolean: return value.toBool();
	case ColumnType::Integer: return value.toInt();
	case ColumnType::Double: return value.toDouble();
	case ColumnType::Uuid:
		if (m_type == Type::Postgres) {
			return QUuid(value.toString());
		} else {
			return QUuid::fromRfc4122(value.toByteArray());
		}
	case ColumnType::TinyString: return value.toString();
	case ColumnType::ShortString: return value.toString();
	case ColumnType::LongString: return value.toString();
	case ColumnType::Text: return value.toString();
	case ColumnType::Blob: return value.toByteArray();
	case ColumnType::Date: return QDate::fromString(value.toString(), Qt::ISODate);
	case ColumnType::DateTime: return QDateTime::fromString(value.toString(), Qt::ISODate);
	case ColumnType::Time: return QTime::fromString(value.toString(), Qt::ISODate);
	case ColumnType::Timestamp: return value.toInt();
	}
}

QString Dialect::columnType(const ColumnType type) const
{
	switch (type) {
	case ColumnType::Serial:
		if (m_type == Type::Postgres) {
			return "SERIAL";
		} else {
			return "INTEGER";
		}
	case ColumnType::Boolean: return "BOOLEAN";
	case ColumnType::Integer: return "INTEGER";
	case ColumnType::Double:
		switch (m_type) {
		case Type::Sqlite: return "REAL";
		case Type::Postgres: return "DOUBLE PRECISION";
		case Type::MySQL: return "DOUBLE";
		}
	case ColumnType::Uuid:
		switch (m_type) {
		case Type::Sqlite: return "BLOB(16)";
		case Type::Postgres: return "UUID";
		case Type::MySQL: return "BINARY(16)";
		}
	case ColumnType::TinyString: return "VARCHAR(16)";
	case ColumnType::ShortString: return "VARCHAR(64)";
	case ColumnType::LongString: return "VARCHAR(256)";
	case ColumnType::Text: return "TEXT";
	case ColumnType::Blob:
		switch (m_type) {
		case Type::Sqlite: return "BLOB";
		case Type::Postgres: return "BYTEA";
		case Type::MySQL: return "LONGBLOB";
		}
	case ColumnType::Date: return "DATE";
	case ColumnType::DateTime: return "DATETIME";
	case ColumnType::Time: return "TIME";
	case ColumnType::Timestamp: return "TIMESTAMP";
	}
}

QString Dialect::autoIncrement() const
{
	if (m_type == Type::Postgres) {
		return "";
	} else if (m_type == Type::Sqlite) {
		return "AUTOINCREMENT";
	} else if (m_type == Type::MySQL) {
		return "AUTO_INCREMENT";
	} else {
		throw DatabaseException("Unhandled database type, don't know the dialect");
	}
}

QString Dialect::valueToString(const QVariant &value) const
{
	switch (value.type()) {
	case QVariant::String: return '\'' + value.toString() + '\'';
	case QVariant::Date: return '\'' + value.toDate().toString(Qt::ISODate) + '\'';
	case QVariant::DateTime: return '\'' + value.toDateTime().toString(Qt::ISODate) + '\'';
	case QVariant::Time: return '\'' + value.toTime().toString(Qt::ISODate) + '\'';
	case QVariant::Bool: return value.toBool() ? "TRUE" : "FALSE";
	case QVariant::Uuid: return '\'' + QString::fromLatin1("0x" + value.toUuid().toRfc4122().toHex()) + '\'';
	case QVariant::ByteArray: return '\'' + QString::fromLatin1("0x" + value.toByteArray().toHex()) + '\'';
	default: return value.toString();
	}
}

}
