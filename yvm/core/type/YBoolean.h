#pragma once
#ifndef __YBOOLEAN_H__
#define __YBOOLEAN_H__
#include <iostream>
#include <sstream>

namespace ytype {
    class YBoolean {
        bool value = false;

    public:
        YBoolean(bool v);
        YBoolean()=default;

        std::string toString();
        int toInteger();
        float toDecimal();

        bool get();
        void set(bool v);
    };
}

#endif
