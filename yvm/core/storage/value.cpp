#include "value.h"

ysto::Value::Value(ytype::YInteger v, bool isc, int ln, int col): integerValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::integer, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YBoolean v, bool isc, int ln, int col): booleanValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::boolean, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YString v, bool isc, int ln, int col): stringValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::string, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YDecimal v, bool isc, int ln, int col): decimalValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::decimal, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YObject v, bool isc, int ln, int col): objectValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::object, ytype::compType::norm}) {}
ysto::Value::Value(int ln, int col) {
    type = {ytype::basicType::null, ytype::compType::norm};
    line = ln, column = col;
}
ysto::Value::Value(std::vector<Value> v, bool isc, int ln, int col): list(v), isConstant(isc), line(ln), column(col), type(v[0].getType()), isList(true) {}

ysto::Value::Value(ytype::YInteger v, bool isc, bool isdyn, int ln, int col): integerValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::integer, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YBoolean v, bool isc, bool isdyn, int ln, int col): booleanValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::boolean, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YString v, bool isc, bool isdyn, int ln, int col): stringValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::string, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YDecimal v, bool isc, bool isdyn, int ln, int col): decimalValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::decimal, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YObject v, bool isc, bool isdyn, int ln, int col): objectValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::object, ytype::compType::norm}) {}
ysto::Value::Value(bool isdyn, int ln, int col) {
    type = {ytype::basicType::null, ytype::compType::norm};
    isDynamic = isdyn;
    line = ln, column = col;
}
ysto::Value::Value(std::vector<Value> v, bool isc, bool isdyn, int ln, int col): list(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type(v[0].getType()), isList(true) {}

ysto::Value::Value(std::string content) {
    stringValue = ytype::YString(content);
    type = {ytype::basicType::flag, ytype::compType::norm};
}
ytype::basicType& ysto::Value::getBasicType() {
    return type.bt;
}
ytype::compType& ysto::Value::getCompType() {
    return type.ct;
}
ytype::ytypeUnit& ysto::Value::getType() {
    return type;
}

bool ysto::Value::isConst() {
    return isConstant;
}

bool ysto::Value::isListValue() {
    return isList;
}

bool ysto::Value::isDyn() {
    return isDynamic;
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
