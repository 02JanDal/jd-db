#include "SelectBuilder.h"

SelectBuilder::SelectBuilder(const Table &table, const QVector<QString> &columns)
	: m_columns(columns), m_table(table) {}

SelectBuilder &SelectBuilder::orderBy(const QString &col, const Qt::SortOrder order)
{
	Q_ASSERT_X(m_table.columns().contains(col), "SelectBuilder::orderBy", "attempt to order by unknown column");
	m_sorting = qMakePair(col, order);
	return *this;
}
SelectBuilder &SelectBuilder::where(const QString &condition)
{
	m_where = condition;
	return *this;
}
SelectBuilder &SelectBuilder::updatedSince(const int since)
{
	m_updatedSince = since;
	return *this;
}
SelectBuilder &SelectBuilder::limit(const int limit)
{
	m_limit = limit;
	return *this;
}
SelectBuilder &SelectBuilder::offset(const int offset)
{
	m_offset = offset;
	return *this;
}

QString SelectBuilder::toSql() const
{
	QString where = m_where;
	if (m_updatedSince != -1) {
		where += QString(where.isNull() ? "" : " AND ") + "updated_at >= " + QString::number(m_updatedSince);
	}

	return "SELECT " + m_columns.toList().join(',') + " FROM " + m_table.tableName()
			+ (where.isNull() ? ""
							  : " WHERE " + where)
			+ (m_sorting.first.isNull() ? ""
										: " ORDER BY " + m_sorting.first + (m_sorting.second == Qt::AscendingOrder ? " ASC" : " DESC"))
			+ (m_limit == -1 ? ""
							 : " LIMIT " + QString::number(m_limit) + (m_offset == -1 ? ""
																					  : " OFFSET " + QString::number(m_offset)));
}

SelectBuilder selectFrom(const Table &table, const QVector<QString> &columns)
{
	return SelectBuilder(table, columns.isEmpty() ? table.columns().keys().toVector() : columns);
}
