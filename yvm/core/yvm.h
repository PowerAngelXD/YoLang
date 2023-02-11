#pragma once
#include "storage/space.h"

namespace vmcore {
    ysto::Value gwv(ysto::Value value); // 获取总体值
    byteCode objCode2ByteCode(ytype::byteCode); // 将object中使用的伪bytecode转换成vm能够处理的bytecode


    namespace native {
        static ysto::Value null_value(0, 0);

        class BuiltInFunctionSet {
        public:
            static ysto::Value println(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value input(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value fread(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value fwrite(std::vector<ysto::Value> args,byteCode code);
            static ysto::Value substr(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value ref(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value vmcode(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value add_const(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value length(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value randint(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value rand_deci(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value randstr(std::vector<ysto::Value> args, byteCode code);
            static ysto::Value split(std::vector<ysto::Value> args, byteCode code);
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

        std::vector<byteCode> mainQueue; // 主代码队列
        std::vector<std::vector<byteCode>> codeQueue; // 代码队列

        // 指令对应的函数实现
        void _new(byteCode code);
        void gmem(byteCode code);
        void point_to(byteCode code);
        void flag(byteCode code);
        void tcast(byteCode code);
        void add(byteCode code);
        void sub(byteCode code);
        void div(byteCode code);
        void mul(byteCode code);
        void mod(byteCode code);
        void stf(byteCode code);
        void lt(byteCode code);
        void gt(byteCode code);
        void ltet(byteCode code);
        void gtet(byteCode code);
        void tequ(byteCode code);
        void no(byteCode code);
        void logicAnd(byteCode code);
        void logicOr(byteCode code);
        void selfadd(byteCode code);
        void selfsub(byteCode code);
        void equ(byteCode code);
        void noequ(byteCode code);
        void idx(byteCode code);
        void out(byteCode code);
        void lst(byteCode code);
        void lstend(byteCode code);
        void paraend(byteCode code);
        void push(byteCode code);
        void create(byteCode code, int &current);
        void assign(byteCode code);
        void scopestart(byteCode code);
        void scopeend(byteCode code);
        int jmp(byteCode code,  std::vector<byteCode>& queue, int current);
        void del_val();
        void del(byteCode code);
        void idenend(byteCode code);
        void call(byteCode code, std::string arg);
        //
    public:
        ysto::Space space; // 全局Space
        native::navtiveSet native;

        vm(std::vector<byteCode> cs, std::vector<std::string> cp);
        vm()=default;

        void run(int queue_id, std::string arg); // queue指代要运行哪条栈的代码，-1为主栈
        ysto::Value getResult();
        void load(std::vector<std::string> cp, std::vector<byteCode> cs);
    };
}