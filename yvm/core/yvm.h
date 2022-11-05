#pragma once
#include "storage/space.h"

namespace vmcore {
    ysto::Value gwv(ysto::Value value); // 获取总体值
    ysto::Value gwvRef(ysto::Value value); // 获取总体值

    namespace native {
        static ysto::Value null_value(0, 0);

        class BuiltInFunctionSet {
        public:
            static ysto::Value println(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value input(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value fread(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value fwrite(std::vector<ysto::Value> args,ygen::byteCode code);
            static ysto::Value substr(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value ref(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value vmcode(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value add_const(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value length(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value randint(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value rand_deci(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value randstr(std::vector<ysto::Value> args, ygen::byteCode code);
            static ysto::Value split(std::vector<ysto::Value> args, ygen::byteCode code);
        };

        class BuiltInStructSet {
        public:
            static ysto::Value Point();
            static ysto::Value Application();
        };

        class NativeClassSet {
        public:
            ysto::Value Process();
            ysto::Value File();
            ysto::Value IO();
        };

        class BuiltInStructInstanceSet {
        public:
            static ysto::Value appFromApplication();
        };

        struct navtiveSet {
            std::vector<std::string> nativeIdenList= {"Point", "Application", "yolang", "releaseVersion", "completeVersion", "_integer", "_decimal", "_string", "_boolean"};
            BuiltInStructInstanceSet bisiSet;
            BuiltInStructSet bisSet;
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

        // 指令对应的函数实现
        void _new(ygen::byteCode code);
        void gmem(ygen::byteCode code);
        void flag(ygen::byteCode code);
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
        void tequ(ygen::byteCode code);
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
        void create(ygen::byteCode code, int &current);
        void assign(ygen::byteCode code);
        void scopestart(ygen::byteCode code);
        void scopeend(ygen::byteCode code);
        int jmp(ygen::byteCode code,  std::vector<ygen::byteCode>& queue, int current);
        void del_val();
        void del(ygen::byteCode code);
        void idenend(ygen::byteCode code);
        void call(ygen::byteCode code, std::string arg);
        //
    public:
        ysto::Space space; // 全局Space
        native::navtiveSet native;

        vm(std::vector<ygen::byteCode> cs, std::vector<std::string> cp);
        vm()=default;

        void run(int queue_id, std::string arg); // queue指代要运行哪条栈的代码，-1为主栈
        ysto::Value getResult();
        void load(std::vector<std::string> cp, std::vector<ygen::byteCode> cs);
    };
}