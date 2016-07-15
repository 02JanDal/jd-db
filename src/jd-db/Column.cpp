#include "Column.h"

#include <QUuid>
#include <QDate>
#include <QDateTime>
#include <QTime>

#include "Table.h"

static QString sqlValue(const SqlDialectType type, const QVariant &val)
{
	if (val.type() == QVariant::String || val.type() == QVariant::Date || val.type() == QVariant::DateTime || val.type() == QVariant::Time) {
		return "'" + val.toString() + "'";
	} else if (val.type() == QVariant::Bool) {
		return val.toBool() ? "TRUE" : "FALSE";
	} else if (val.isNull()) {
		return "NULL";
	} else if (val.type() == QVariant::ByteArray) {
		return QString::fromLatin1("0x" + val.toByteArray().toHex());
	} else {
		return val.toString();
	}
}
static int typeIdFromSql(const QString &sql)
{
	if (sql.startsWith("VARCHAR") || sql.startsWith("CHAR")) {
		return QVariant::String;
	} else if (sql == "DATETIME") {
		return QVariant::DateTime;
	} else if (sql == "TIME") {
		return QVariant::Time;
	} else if (sql == "DATE") {
		return QVariant::Date;
	} else if (sql == "TIMESTAMP" || sql.startsWith("INT") || sql.startsWith("SERIAL")) {
		return QVariant::LongLong;
	} else if (sql == "BOOLEAN") {
		return QVariant::Bool;
	} else if (sql == "BLOB" || sql == "BYTEA") {
		return QVariant::ByteArray;
	} else {
		return QVariant::Invalid;
	}
}
static QString idColumnType(const SqlDialectType type)
{
	switch (type) {
	case SqlDialectType::Sqlite: return "BLOB(16)";
	case SqlDialectType::Postgres: return "UUID";
	case SqlDialectType::MySQL: return "BINARY(16)";
	}
}

Column::Column(const SqlDialectType sqltype, const QString &name, const QString &type)
	: Column(sqltype, name, type, typeIdFromSql(type)) {}
Column::Column(const SqlDialectType sqltype, const Table &table)
	: Column(sqltype, table.tableName() + "_id", idColumnType(sqltype)) {}
Column::Column(const SqlDialectType sqltype, const QString &name, const QString &type, const int &typeId)
	: m_sqltype(sqltype), m_name(name), m_type(type), m_typeId(typeId) {}

QString Column::sql() const
{
	// i heard you like the ternary operator?

	const QString type = m_name == "id" ? idColumnType(m_sqltype) + " PRIMARY KEY" : m_type;

	return "'" + m_name + "' " + type
			+ (m_unique ? " UNIQUE" : "")
			+ (m_nullable ? " NULL" : " NOT NULL")
			+ (!m_default.isValid() ? "" : (" DEFAULT " + sqlValue(m_sqltype, toDb(m_default))))
			+ (m_tableRef.isNull()
			   ? ""
			   : m_sqltype == SqlDialectType::Sqlite ? " REFERENCES " + m_tableRef + "(id)"
													 : ", FOREIGN KEY (" + m_name + ") REFERENCES " + m_tableRef + "(id)");
}

QVariant Column::toVariant(const QVariant &dbVariant) const
{
	switch (m_typeId) {
	case QVariant::Uuid:
		if (m_sqltype == SqlDialectType::Postgres) {
			return QUuid(dbVariant.toString());
		} else {
			return QUuid::fromRfc4122(dbVariant.toByteArray());
		}
	case QVariant::Date: return QDate::fromString(dbVariant.toString(), Qt::ISODate);
	case QVariant::DateTime: return QDateTime::fromString(dbVariant.toString(), Qt::ISODate);
	case QVariant::Time: return QTime::fromString(dbVariant.toString(), Qt::ISODate);
	default:
		return dbVariant;
	}
}

QVariant Column::toDb(const QVariant &variant) const
{
	if (m_typeId == QVariant::Uuid) {
		if (m_sqltype == SqlDialectType::Postgres) {
			return variant.toUuid().toString();
		} else {
			return variant.toUuid().toRfc4122();
		}
	}
	return variant;
}
