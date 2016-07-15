#pragma once

#include "Table.h"

class SelectBuilder
{
public:
	explicit SelectBuilder(const Table &table, const QVector<QString> &columns);

	SelectBuilder &orderBy(const QString &col, const Qt::SortOrder order = Qt::AscendingOrder);
	SelectBuilder &where(const QString &condition);
	SelectBuilder &updatedSince(const int since);
	SelectBuilder &limit(const int limit);
	SelectBuilder &offset(const int offset);

	operator QString() const { return toSql(); }
	QString toSql() const;

private:
	QVector<QString> m_columns;
	Table m_table;
	QPair<QString, Qt::SortOrder> m_sorting;
	QString m_where;
	int m_updatedSince = -1;
	int m_limit = -1;
	int m_offset = -1;
};
