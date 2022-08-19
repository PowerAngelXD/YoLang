#pragma once
#ifndef __YDECIMAL_H__
#define __YDECIMAL_H__
#include <iostream>
#include <sstream>

namespace ytype {
    class YDecimal {
        float value = 0;

    public:
        YDecimal(float v);
        YDecimal()=default;

        std::string toString();
        int toInteger();
        bool toBoolean();

        float& get();
        void set(float v);
        void operator=(float v);
    };
}

#endif
