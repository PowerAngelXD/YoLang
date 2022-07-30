#pragma once
#ifndef __YSTRING_H__
#define __YSTRING_H__
#include <iostream>
#include <sstream>

namespace ytype {
    class YString {
        std::string value = "\0";

    public:
        YString(std::string v);
        YString()=default;

        float toDecimal();
        int toInteger();
        bool toBoolean();

        std::string get();
        void set(std::string v);
        void operator=(std::string v);
    };
}

#endif
