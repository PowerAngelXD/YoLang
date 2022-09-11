#ifndef YNULL_H
#define YNULL_H
#include <iostream>
#include <sstream>

namespace ytype {
    class YNull {
    public:
        YNull()=default;

        std::string toString();
        int toInteger();
        float toDecimal();
        bool toBoolean();
    };
}

#endif
