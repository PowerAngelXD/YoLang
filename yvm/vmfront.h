#include "core/yvm.h"

namespace yvm{
    // 工具库
    namespace tools{
        /**
         * @brief 分割字符串函数
         *
         * @param str 要分割的字符串
         * @param sp 分割character
         * @return std::vector<std::string> 一个vector
         */
        std::vector<std::string> split(std::string str, char sp);
        /**
         * @brief 判断所给文件名是否符合某种格式的文件
         *
         * @param name 文件名
         * @param type 文件扩展名
         * @return true
         * @return false
         */
        bool compareFileType(std::string name, std::string type);
        /**
         * @brief 用于给ast字符串格式化的函数
         * @param aststr ast字符串
         * @return
         */
        std::string formatAst(std::string aststr);
        std::string mulStr(std::string str, int times);
        std::string stmtsToString(std::vector<AST::StmtNode*> stmts);
    }

    // vm前端，有各个关于vm的操作
    class vmfront {
        vmcore::vm vm; // vm实例
    public:
        std::string compeleteVersion = "Release-2022-11-0169";
        std::string version = "22.11.169";
        std::string releaseVersion = "Yolang 1.5.9";

        vmfront() = default;

        vmfront& newValue(std::string name, ysto::Value value);
        vmfront& clearVM(); // clearVM只会把vm中的globalspace内的所有内容清空
        vmfront& runVM(std::string arg);
        vmfront& setupVM();
        vmfront& resetVM(std::vector<ygen::byteCode> codes, std::vector<std::string> consts); // reset只会把vm中的codes和constPool覆写一遍
        vmcore::vm getVM();
    };

    //ysh
    typedef void (*ins_method)(std::vector<std::string>);
    typedef std::vector<std::string> ins_method_para;

    class yshBase {
    public:
        yshBase();

        std::string shellVersion;
        vmfront front;
        parser::Parser yparser; // 一个parser

        std::vector<yolexer::yoToken> makeTokenGroup(std::string text);
        virtual void yshStart(); // 需要继承base类的类进行覆写
        virtual void yshUpdate();
        virtual void yshEnd();

        virtual void catchYoError(yoexception::YoError ye);
    };

    class InsSet {
    public:
        virtual void repl(ins_method_para para, yshBase* ysh); // 规定：每一个ysh程序必须有一个repl
        virtual void help(ins_method_para para, yshBase* ysh); // 规定：每一个ysh程序必须有一个help指令（的函数实现，指令名称可以不是help）
        virtual void info(ins_method_para para, yshBase* ysh); // 规定，每一个ysh程序必须有一个info指令（的函数实现，指令名称可以不是info）
        virtual void exit(ins_method_para para, yshBase* ysh);
    };

    void start(yshBase* yshObj);
}
