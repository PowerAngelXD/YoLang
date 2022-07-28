#include "value.h"

ysto::Value::Value(ytype::YInteger v, bool isc): integerValue(v), isConstant(isc) {}

ysto::Value::Value(ytype::YBoolean v, bool isc) {}

ysto::Value::Value(ytype::YString v, bool isc) {}

ysto::Value::Value(ytype::YDecimal v, bool isc) {}

ysto::Value::Value(ytype::YObject v, bool isc) {}

ysto::Value::Value(std::vector<Value> v, bool isc) {}

ygen::type::vtypeUnit ysto::Value::getType() {
    return 0;
}

ytype::YObject &ysto::Value::getObjectValue() {

}

ytype::YInteger &ysto::Value::getIntegerValue() {

}

ytype::YDecimal &ysto::Value::getDecimalValue() {

}

ytype::YBoolean &ysto::Value::getBooleanValue() {

}

ytype::YString &ysto::Value::getStringValue() {

}

std::vector<ysto::Value> &ysto::Value::getList() {

}

void ysto::Value::coverList(std::vector<Value> ls) {

}
