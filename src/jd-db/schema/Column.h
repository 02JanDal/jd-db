#pragma once

#include <QString>
#include <QVariant>

class Table;

enum class ColumnType
{
	Serial,
	Boolean,
	Integer,
	Double,
	Uuid,
	TinyString,
	ShortString,
	LongString,
	Text,
	Blob,
	Date,
	DateTime,
	Time,
	Timestamp
};
int typeId(const ColumnType type);
QJsonValue toJson(const ColumnType type, const QVariant &value);
QVariant fromJson(const ColumnType type, const QJsonValue &value);

class Column
{
	QString m_name;
	ColumnType m_type;
	bool m_nullable = false;
	bool m_unique = false;
	QVariant m_default = QVariant::Invalid;
	QString m_tableRef;

public:
	explicit Column(const QString &name, const ColumnType type);
	explicit Column(const Table &table);
	explicit Column() {}

	Column &setNullable(const bool nullable = true) { m_nullable = nullable; return *this; }
	Column &setUnique(const bool unique = true) { m_unique = unique; return *this; }
	Column &setDefault(const QVariant &def) { m_default = def; return *this; }

	QString sql() const;

	QString name() const { return m_name; }
	ColumnType type() const { return m_type; }
	bool isNullable() const { return m_nullable; }
	bool isUnique() const { return m_unique; }
	QVariant defaultValue() const { return m_default; }
	QString referencedTable() const { return m_tableRef; }
	bool isReference() const { return !m_tableRef.isNull(); }
};
