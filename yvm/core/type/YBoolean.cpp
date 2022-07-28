#include "YBoolean.h"

ytype::YBoolean::YBoolean(bool v) {
    value = v;
}

std::string ytype::YBoolean::toString() {
    return value?"true":"false";
}

int ytype::YBoolean::toInteger() {
    return value?1:0;
}

float ytype::YBoolean::toDecimal() {
    return static_cast<float>(value?1:0);
}

bool ytype::YBoolean::get() {
    return value;
}

void ytype::YBoolean::set(bool v) {
    value = v;
}
