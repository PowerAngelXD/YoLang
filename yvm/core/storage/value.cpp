#include "value.h"

ysto::Value::Value(ytype::YInteger v, bool isc, int ln, int col): integerValue(v), isConstant(isc), line(ln), column(col), type(ygen::type::vtype::integer) {}

ysto::Value::Value(ytype::YBoolean v, bool isc, int ln, int col): booleanValue(v), isConstant(isc), line(ln), column(col), type(ygen::type::vtype::boolean) {}

ysto::Value::Value(ytype::YString v, bool isc, int ln, int col): stringValue(v), isConstant(isc), line(ln), column(col), type(ygen::type::vtype::string) {}

ysto::Value::Value(ytype::YDecimal v, bool isc, int ln, int col): decimalValue(v), isConstant(isc), line(ln), column(col), type(ygen::type::vtype::decimal) {}

ysto::Value::Value(ytype::YObject v, bool isc, int ln, int col): objectValue(v), isConstant(isc), line(ln), column(col), type(ygen::type::vtype::object) {}

ysto::Value::Value(std::vector<Value> v, bool isc, int ln, int col): list(v), isConstant(isc), line(ln), column(col) {}

ygen::type::vtypeUnit ysto::Value::getType() {
    return type;
}

ytype::YObject &ysto::Value::getObjectValue() {
    return objectValue;
}

ytype::YInteger &ysto::Value::getIntegerValue() {
    return integerValue;
}

ytype::YDecimal &ysto::Value::getDecimalValue() {
    return decimalValue;
}

ytype::YBoolean &ysto::Value::getBooleanValue() {
    return booleanValue;
}

ytype::YString &ysto::Value::getStringValue() {
    return stringValue;
}

std::vector<ysto::Value> &ysto::Value::getList() {
    return list;
}
