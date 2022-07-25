#pragma once
#include "YObject.cpp"

namespace ytype {
    class YDecimal {
        float value = 0;

    public:
        std::string toString();
        float toDecimal();
        bool toBoolean();

        int get();
        void set(int v);
    };
}
