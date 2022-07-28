#include "YString.h"

ytype::YString::YString(std::string v) {
    value = v;
}

float ytype::YString::toDecimal() {
    return atof(value.c_str());
}

int ytype::YString::toInteger() {
    return atoi(value.c_str());
}

bool ytype::YString::toBoolean() {
    return value == "true"?true:false;
}

std::string ytype::YString::get() {
    return value;
}

void ytype::YString::set(std::string v) {
    value = v;
}
