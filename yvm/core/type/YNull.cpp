#include "YNull.h"

std::string ytype::YNull::toString() {
    return "<null>";
}

int ytype::YNull::toInteger() {
    return 0;
}

float ytype::YNull::toDecimal() {
    return 0.0;
}

bool ytype::YNull::toBoolean() {
    return false;
}
