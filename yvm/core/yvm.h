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
        void tcast(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void add(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void sub(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void div(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void mul(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void mod(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void stf(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void lt(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void gt(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void ltet(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void gtet(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void no(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void logicAnd(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void logicOr(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void selfadd(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void selfsub(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void equ(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void noequ(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void idx(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void out(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void lst(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void lstend(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void paraend(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void push(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void create(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void assign(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void scopestart(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void scopeend(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        int jmp(ygen::byteCode code,  std::vector<ygen::byteCode> queue, int current);
        void del_val(std::vector<ygen::byteCode> queue);
        void del(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void idenend(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        void call(ygen::byteCode code,  std::vector<ygen::byteCode> queue);
        //

        void run(int queue_id, std::string arg); // queue指代要运行哪条栈的代码，-1为主栈
        ysto::Value getResult();
        void load(std::vector<std::string> cp, std::vector<ygen::byteCode> cs);
    };
}