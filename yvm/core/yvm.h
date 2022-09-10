#pragma once
#include "storage/space.h"

namespace vmcore {
    namespace native {
        static ysto::Value null_value(0, 0);

        class BuiltInFunctionSet {
        public:
            ysto::Value println(std::vector<ysto::Value> args, ygen::byteCode code);
            ysto::Value input(std::vector<ysto::Value> args, ygen::byteCode code);
            ysto::Value fread(std::vector<ysto::Value> args, ygen::byteCode code);
            ysto::Value fwrite(std::vector<ysto::Value> args,ygen::byteCode code);
            ysto::Value substr(std::vector<ysto::Value> args, ygen::byteCode code);
            ysto::Value system(std::vector<ysto::Value> args, ygen::byteCode code);
        };

        struct navtiveSet {
            BuiltInFunctionSet bifSet;
        };
    }

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

        std::vector<ygen::byteCode> mainQueue; // 主代码队列
        std::vector<std::vector<ygen::byteCode>> codeQueue; // 代码队列

        native::navtiveSet native;
        ysto::Space space; // 全局Space
    public:
        vm(std::vector<ygen::byteCode> cs, std::vector<std::string> cp);
        vm()=default;

        // 指令对应的函数实现
        void tcast(ygen::byteCode code);
        void add(ygen::byteCode code);
        void sub(ygen::byteCode code);
        void div(ygen::byteCode code);
        void mul(ygen::byteCode code);
        void mod(ygen::byteCode code);
        void stf(ygen::byteCode code);
        void lt(ygen::byteCode code);
        void gt(ygen::byteCode code);
        void ltet(ygen::byteCode code);
        void gtet(ygen::byteCode code);
        void no(ygen::byteCode code);
        void logicAnd(ygen::byteCode code);
        void logicOr(ygen::byteCode code);
        void selfadd(ygen::byteCode code);
        void selfsub(ygen::byteCode code);
        void equ(ygen::byteCode code);
        void noequ(ygen::byteCode code);
        void idx(ygen::byteCode code);
        void out(ygen::byteCode code);
        void lst(ygen::byteCode code);
        void lstend(ygen::byteCode code);
        void paraend(ygen::byteCode code);
        void push(ygen::byteCode code);
        void create(ygen::byteCode code);
        void assign(ygen::byteCode code);
        void scopestart(ygen::byteCode code);
        void scopeend(ygen::byteCode code);
        int jmp(ygen::byteCode code, int current);
        void del_val();
        void del(ygen::byteCode code);
        void idenend(ygen::byteCode code);
        void call(ygen::byteCode code);
        //

        void run(int queue_id, std::string arg); // queue指代要运行哪条栈的代码，-1为主栈
        ysto::Value getResult();
        void load(std::vector<std::string> cp, std::vector<ygen::byteCode> cs);
    };
}