#pragma once
#ifndef __YINTEGER_H__
#define __YINTEGER_H__
#include <iostream>
#include <sstream>

namespace ytype {
    class YInteger {
        int value = 0;

    public:
        YInteger(int v);
        YInteger()=default;

        std::string toString();
        float toDecimal();
        bool toBoolean();

        int& get();
        void set(int v);
        void operator=(int v);
    };
}

#endif