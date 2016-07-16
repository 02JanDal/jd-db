#pragma once

#include "jd-db/schema/Column.h"

class QSqlDatabase;
class QVariant;

namespace DB {
class Dialect
{
public:
	enum class Type {
		Sqlite,
		Postgres,
		MySQL
	};
	explicit Dialect(const Type type);
	Dialect() {}

	Type type() const { return m_type; }

	QVariant convertToDatabase(const QVariant &value) const;
	QVariant convertFromDatabase(const ColumnType type, const QVariant &value) const;

	QString columnType(const ColumnType type) const;
	QString autoIncrement() const;
	QString valueToString(const QVariant &value) const;

	static Dialect fromDb(const QSqlDatabase &db);

private:
	Type m_type;
};
}
