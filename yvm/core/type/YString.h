#pragma once
#include "YObject.h"

namespace ytype {
    class YString {
        std::string value = 0;

    public:
        float toDecimal();
        int toInteger();
        bool toBoolean();

        std::string get();
        void set(std::string v);
    };
}
