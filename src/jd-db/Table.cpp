#include "Table.h"

#include <jd-util/Functional.h>

#include "SelectBuilder.h"

Table::Table(const SqlDialectType type, const QString &table)
	: m_sqltype(type), m_tableName(table)
{
	add("id", "UUID");
	add("updated_at", "TIMESTAMP");
}

Column &Table::add(const QString &name, const QString &type)
{
	m_columns.insert(name, Column(m_sqltype, name, type)); return m_columns[name];
}
Column &Table::add(const Table &other)
{
	m_columns.insert(other.tableName() + "_id", Column(m_sqltype, other)); return m_columns[other.tableName() + "_id"];
}

QString Table::columnsList() const
{
	return JD::Util::Functional::collection(m_columns.values())
			.map([](const Column &c) { return c.name(); })
			.join(',');
}

QString Table::createStatement() const
{
	return "CREATE TABLE IF NOT EXISTS " + m_tableName + " ("
			+ JD::Util::Functional::collection(m_columns.values()).map([](const Column &col) { return col.sql(); }).join(',')
			+ ")";
}

SelectBuilder Table::selectFrom(const QVector<QString> &columns) const
{
	return SelectBuilder(*this, columns.isEmpty() ? m_columns.keys().toVector() : columns);
}
