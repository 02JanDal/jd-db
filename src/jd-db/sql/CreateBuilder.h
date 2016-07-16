#pragma once

#include "jd-db/sql/Database.h"

class Table;

namespace DB {
class Dialect;

QString createStatement(const Table &table, const Dialect &dialect);
}
