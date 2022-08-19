#include "YInteger.h"

ytype::YInteger::YInteger(int v) {
    value = v;
}

std::string ytype::YInteger::toString() {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

float ytype::YInteger::toDecimal() {
    return static_cast<float>(value);
}

bool ytype::YInteger::toBoolean() {
    return value == 1?true:false;
}

int& ytype::YInteger::get() {
    return value;
}

void ytype::YInteger::set(int v) {
    value = v;
}

void ytype::YInteger::operator=(int v) {
    value = v;
}
