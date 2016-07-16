#include "Column.h"

#include <QUuid>
#include <QDate>
#include <QDateTime>
#include <QTime>

#include <jd-util/Json.h>

#include "Table.h"

static ColumnType foreignKeyType(const Table &other)
{
	const Column otherCol = other.column("id");
	if (otherCol.type() == ColumnType::Serial) {
		return ColumnType::Integer;
	} else {
		return otherCol.type();
	}
}

Column::Column(const QString &name, const ColumnType type)
	: m_name(name), m_type(type) {}
Column::Column(const Table &table)
	: Column(table.tableName() + "_id", foreignKeyType(table)) {}

int typeId(const ColumnType type)
{
	switch (type) {
	case ColumnType::Serial: return QVariant::Int;
	case ColumnType::Boolean: return QVariant::Bool;
	case ColumnType::Integer: return QVariant::Int;
	case ColumnType::Double: return QVariant::Double;
	case ColumnType::Uuid: return QVariant::Uuid;
	case ColumnType::TinyString: return QVariant::String;
	case ColumnType::ShortString: return QVariant::String;
	case ColumnType::LongString: return QVariant::String;
	case ColumnType::Text: return QVariant::String;
	case ColumnType::Blob: return QVariant::ByteArray;
	case ColumnType::Date: return QVariant::Date;
	case ColumnType::DateTime: return QVariant::DateTime;
	case ColumnType::Time: return QVariant::Time;
	case ColumnType::Timestamp: return QVariant::Int;
	}
}
QJsonValue toJson(const ColumnType type, const QVariant &value)
{
	if (value.isNull()) {
		return QJsonValue::Null;
	}

	switch (type) {
	case ColumnType::Serial: return Json::toJson(value.toInt());
	case ColumnType::Boolean: return Json::toJson(value.toBool());
	case ColumnType::Integer: return Json::toJson(value.toInt());
	case ColumnType::Double: return Json::toJson(value.toDouble());
	case ColumnType::Uuid: return Json::toJson(value.toUuid());
	case ColumnType::TinyString: return Json::toJson(value.toString());
	case ColumnType::ShortString: return Json::toJson(value.toString());
	case ColumnType::LongString: return Json::toJson(value.toString());
	case ColumnType::Text: return Json::toJson(value.toString());
	case ColumnType::Blob: return Json::toJson(value.toByteArray());
	case ColumnType::Date: return Json::toJson(value.toDate());
	case ColumnType::DateTime: return Json::toJson(value.toDateTime());
	case ColumnType::Time: return Json::toJson(value.toTime());
	case ColumnType::Timestamp: return Json::toJson(value.toInt());
	}
}
QVariant fromJson(const ColumnType type, const QJsonValue &value)
{
	if (value.isNull()) {
		return QVariant();
	}

	switch (type) {
	case ColumnType::Serial: return Json::ensureInteger(value);
	case ColumnType::Boolean: return Json::ensureBoolean(value);
	case ColumnType::Integer: return Json::ensureInteger(value);
	case ColumnType::Double: return Json::ensureDouble(value);
	case ColumnType::Uuid: return Json::ensureUuid(value);
	case ColumnType::TinyString: return Json::ensureString(value);
	case ColumnType::ShortString: return Json::ensureString(value);
	case ColumnType::LongString: return Json::ensureString(value);
	case ColumnType::Text: return Json::ensureString(value);
	case ColumnType::Blob: return Json::ensureByteArray(value);
	case ColumnType::Date: return Json::ensureDate(value);
	case ColumnType::DateTime: return Json::ensureDateTime(value);
	case ColumnType::Time: return Json::ensureTime(value);
	case ColumnType::Timestamp: return Json::ensureInteger(value);
	}
}
