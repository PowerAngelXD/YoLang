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

    class vm {
        std::vector<std::string> constPool; // 存放字符串的池
        YStack<ysto::Value> valueStack; // 存放value的栈
        std::vector<ygen::byteCode> codes; // 传入的中间码序列
        ysto::Space space; // 全局Space
    public:
        vm(std::vector<ygen::byteCode> cs, std::vector<std::string> cp);
        vm()=default;

        // 指令对应的函数实现
        void add(ygen::byteCode code);
        void push(ygen::byteCode code);
        //

        void run(std::string arg);
        ysto::Value getResult();
    };
}