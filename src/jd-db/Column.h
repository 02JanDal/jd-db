#pragma once

#include <QString>
#include <QVariant>

enum class SqlDialectType
{
	Sqlite,
	Postgres,
	MySQL
};

class Table;

class Column
{
	SqlDialectType m_sqltype;
	QString m_name;
	QString m_type;
	int m_typeId;
	bool m_nullable = false;
	bool m_unique = false;
	QVariant m_default = QVariant::Invalid;
	QString m_tableRef;

public:
	explicit Column(const SqlDialectType sqltype, const QString &name, const QString &type);
	explicit Column(const SqlDialectType sqltype, const QString &name, const QString &type, const int &typeId);
	explicit Column(const SqlDialectType sqltype, const Table &table);
	explicit Column() {}

	Column &setNullable(const bool nullable = true) { m_nullable = nullable; return *this; }
	Column &setUnique(const bool unique = true) { m_unique = unique; return *this; }
	Column &setDefault(const QVariant &def) { m_default = def; return *this; }

	QString sql() const;

	QString name() const { return m_name; }
	QString type() const { return m_type; }
	int typeId() const { return m_typeId; }
	bool isNullable() const { return m_nullable; }
	bool isUnique() const { return m_unique; }
	QVariant defaultValue() const { return m_default; }
	QString referencedTable() const { return m_tableRef; }
	bool isReference() const { return !m_tableRef.isNull(); }
};
