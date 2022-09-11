#include "value.h"

ysto::Value::Value(ytype::YInteger v, bool isc, int ln, int col): integerValue(v), isConstant(isc), line(ln), column(col), type(ytype::vtype::integer) {}
ysto::Value::Value(ytype::YBoolean v, bool isc, int ln, int col): booleanValue(v), isConstant(isc), line(ln), column(col), type(ytype::vtype::boolean) {}
ysto::Value::Value(ytype::YString v, bool isc, int ln, int col): stringValue(v), isConstant(isc), line(ln), column(col), type(ytype::vtype::string) {}
ysto::Value::Value(ytype::YDecimal v, bool isc, int ln, int col): decimalValue(v), isConstant(isc), line(ln), column(col), type(ytype::vtype::decimal) {}
ysto::Value::Value(ytype::YObject v, bool isc, int ln, int col): objectValue(v), isConstant(isc), line(ln), column(col), type(ytype::vtype::object) {}
ysto::Value::Value(std::string content) {
    stringValue = ytype::YString(content);
    type = ytype::vtype::flag;
}
ysto::Value::Value(int ln, int col) {
    type = ytype::vtype::null;
    line = ln, column = col;
}
ysto::Value::Value(std::vector<Value> v, bool isc, int ln, int col): list(v), isConstant(isc), line(ln), column(col), type(v[0].getType()), isList(true) {}

ytype::vtype& ysto::Value::getType() {
    return type;
}

bool ysto::Value::isConst() {
    return isConstant;
}

bool ysto::Value::isListValue() {
    return isList;
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

ytype::YNull &ysto::Value::getNullValue() {
    return nullValue;
}
