#include "YDecimal.h"

ytype::YDecimal::YDecimal(float v) {
    value = v;
}

std::string ytype::YDecimal::toString() {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

int ytype::YDecimal::toInteger() {
    return static_cast<int>(value);
}

bool ytype::YDecimal::toBoolean() {
    return value == 1?true:false;
}

float ytype::YDecimal::get() {
    return value;
}

void ytype::YDecimal::set(float v) {
    value = v;
}
