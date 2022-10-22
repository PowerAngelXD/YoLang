#include "core/yvm.h"

namespace yvm{
    // vm前端，有各个关于vm的操作
    class vmfront {
        vmcore::vm vm; // vm实例
    public:
        std::string compeleteVersion = "Instable-2022-10-0150";
        std::string version = "22.10.150";
        std::string releaseVersion = "Yolang 1.5.29";

        vmfront() = default;

        vmfront& newValue(std::string name, ysto::Value value);
        vmfront& clearVM(); // clearVM只会把vm中的globalspace内的所有内容清空
        vmfront& runVM(std::string arg);
        vmfront& setupVM();
        vmfront& resetVM(std::vector<ygen::byteCode> codes, std::vector<std::string> consts); // reset只会把vm中的codes和constPool覆写一遍
    };

    //ysh
    typedef void (*ins_method)(std::vector<std::string>);
    typedef std::vector<std::string> ins_method_para;

    class InsSet {
    public:
        virtual void __repl__(ins_method_para para)=0; // 规定：每一个ysh程序必须有一个repl
        virtual void __help__(ins_method_para para)=0; // 规定：每一个ysh程序必须有一个help指令（的函数实现，指令名称可以不是help）
        virtual void __info__(ins_method_para para)=0; // 规定，每一个ysh程序必须有一个info指令（的函数实现，指令名称可以不是info）
    };

    class yshBase {
    public:
        vmfront front;
        parser::Parser yparser; // 一个parser

        std::vector<yolexer::yoToken> makeTokenGroup(std::string text);
        virtual void yshStart(); // 需要继承base类的类进行覆写
        virtual void yshUpdate();
        virtual void yshEnd();

        virtual void catchYoError(yoexception::YoError ye);
    };

    void start(yshBase yshObj);
}
