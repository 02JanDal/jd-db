#pragma once

#include <QVector>
#include "Column.h"

class SelectBuilder;

class Table
{
	QString m_tableName;
	QHash<QString, Column> m_columns;
public:
	explicit Table(const QString &table);
	explicit Table() {}

	Column &add(const QString &name, const ColumnType type);
	Column &add(const Table &other);

	QHash<QString, Column> columns() const { return m_columns; }
	const Column column(const QString &name) const { return m_columns.value(name); }
	bool contains(const QString &column) const { return m_columns.contains(column); }

	QString columnsList() const;
	QString tableName() const { return m_tableName; }
};
