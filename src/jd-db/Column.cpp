#include "Column.h"

#include "Table.h"

static QString sqlValue(const SqlDialectType type, const QVariant &val)
{
	if (val.type() == QVariant::String || val.type() == QVariant::Date || val.type() == QVariant::DateTime || val.type() == QVariant::Time) {
		return "'" + val.toString() + "'";
	} else if (val.type() == QVariant::Bool) {
		return val.toBool() ? "TRUE" : "FALSE";
	} else if (val.isNull()) {
		return "NULL";
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
	case SqlDialectType::Sqlite: return "INTEGER PRIMARY KEY AUTOINCREMENT";
	case SqlDialectType::Postgres: return "SERIAL PRIMARY KEY";
	case SqlDialectType::MySQL: return "INTEGER PRIMARY KEY AUTO_INCREMENT";
	}
}

Column::Column(const SqlDialectType sqltype, const QString &name, const QString &type)
	: Column(sqltype, name, type, typeIdFromSql(type)) {}
Column::Column(const SqlDialectType sqltype, const Table &table)
	: Column(sqltype, table.tableName() + "_id", "INTEGER") {}
Column::Column(const SqlDialectType sqltype, const QString &name, const QString &type, const int &typeId)
	: m_sqltype(sqltype), m_name(name), m_type(type), m_typeId(typeId) {}

QString Column::sql() const
{
	// i heard you like the ternary operator?

	const QString type = m_name == "id" ? idColumnType(m_sqltype) : m_type;

	return "'" + m_name + "' " + type
			+ (m_unique ? " UNIQUE" : "")
			+ (m_nullable ? " NULL" : " NOT NULL")
			+ (!m_default.isValid() ? "" : (" DEFAULT " + sqlValue(m_sqltype, m_default)))
			+ (m_tableRef.isNull()
			   ? ""
			   : m_sqltype == SqlDialectType::Sqlite ? " REFERENCES " + m_tableRef + "(id)"
												  : ", FOREIGN KEY (" + m_name + ") REFERENCES " + m_tableRef + "(id)");
}
