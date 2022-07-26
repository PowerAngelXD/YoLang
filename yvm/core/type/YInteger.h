#pragma once
#include "YObject.h"

namespace ytype {
    class YInteger {
        int value = 0;

    public:
        YInteger(int v);

        std::string toString();
        float toDecimal();
        bool toBoolean();

        int get();
        void set(int v);
    };
}