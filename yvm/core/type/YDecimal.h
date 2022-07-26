#pragma once
#include "YObject.cpp"

namespace ytype {
    class YDecimal {
        float value = 0;

    public:
        YDecimal(float v);

        std::string toString();
        int toInteger();
        bool toBoolean();

        float get();
        void set(float v);
    };
}
