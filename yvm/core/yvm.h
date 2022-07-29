#pragma once
#include "storage/space.h"

namespace vmcore {
    template <typename Type>
    class YStack {
        std::vector<Type> stack;
    public:
        YStack() = default;

        Type pop();
        Type peek();
        void push(Type value);
    };
}