#include "Table.h"

#include <jd-util/Functional.h>

Table::Table(const QString &table)
	: m_tableName(table)
{
	add("id", ColumnType::Uuid);
	add("updated_at", ColumnType::Timestamp);
}

Column &Table::add(const QString &name, const ColumnType type)
{
	m_columns.insert(name, Column(name, type));
	return m_columns[name];
}
Column &Table::add(const Table &other)
{
	m_columns.insert(other.tableName() + "_id", Column(other));
	return m_columns[other.tableName() + "_id"];
}
