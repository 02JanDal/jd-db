#pragma once

#include <QVector>
#include "Column.h"

class SelectBuilder;

class Table
{
	SqlDialectType m_sqltype;
	QString m_tableName;
	QHash<QString, Column> m_columns;
public:
	explicit Table(const SqlDialectType type, const QString &table);
	explicit Table() {}

	Column &add(const QString &name, const QString &type);
	Column &add(const Table &other);

	QHash<QString, Column> columns() const { return m_columns; }
	const Column column(const QString &name) const { return m_columns.value(name); }
	QString columnsList() const;
	QString tableName() const { return m_tableName; }

	QString createStatement() const;
	SelectBuilder selectFrom(const QVector<QString> &columns = QVector<QString>()) const;
};
