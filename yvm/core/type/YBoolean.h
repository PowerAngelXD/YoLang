#pragma once
#include "YObject.h"

namespace ytype {
    class YBoolean {
        bool value = false;

    public:
        YBoolean(bool v);

        std::string toString();
        int toInteger();
        float toDecimal();

        bool get();
        void set(bool v);
    };
}
