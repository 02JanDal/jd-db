#include "CreateBuilder.h"

#include <jd-util/Functional.h>

#include "jd-db/schema/Table.h"
#include "jd-db/sql/Dialects.h"

QString DB::createStatement(const Table &table, const Dialect &dialect)
{
	auto columnStatement = [dialect](const Column &col)
	{
		// i heard you like the ternary operator?

		const QString type = col.name() == "id" ?
					dialect.columnType(col.type()) + " PRIMARY KEY " + dialect.autoIncrement()
				  : dialect.columnType(col.type());

		return "'" + col.name() + "' " + type
				+ (col.isUnique() ? " UNIQUE" : "")
				+ (col.isNullable() ? " NULL" : " NOT NULL")
				+ (!col.defaultValue().isValid() ? "" : (" DEFAULT " + dialect.valueToString(dialect.convertToDatabase(col.defaultValue()))))
				+ (col.referencedTable().isNull()
				   ? ""
				   : dialect.type() == Dialect::Type::Sqlite ? " REFERENCES " + col.referencedTable() + "(id)"
														 : ", FOREIGN KEY (" + col.name() + ") REFERENCES " + col.referencedTable() + "(id)");
	};

	return "CREATE TABLE IF NOT EXISTS " + table.tableName() + " ("
			+ JD::Util::Functional::collection(table.columns().values()).map(columnStatement).join(',')
			+ ")";
}
