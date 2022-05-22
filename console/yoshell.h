#pragma once
#include "../vm/yvm.h"

namespace ysh{
    // Tools
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
    }

    // 版本号
    static std::string version = "22.5.33";
    // 完整版本
    static std::string completeVersion = "Release-2022-5-0033";
    // 指令方法类型
    typedef void (*insMethod)(std::vector<std::string>);
    // 指令定义
    typedef std::pair<std::string, insMethod> ins;
    // 指令池
    // 指令方法命名规范：insXXX（XXX为指令名称，首字母大写）
    void insHelp(std::vector<std::string> paras);
    void insInfo(std::vector<std::string> paras);
    void insExit(std::vector<std::string> paras);
    void insRun(std::vector<std::string> paras);
    void insGen(std::vector<std::string> paras);
    void insEnv(std::vector<std::string> paras);
    void insEVAL(std::vector<std::string> paras); // 用于执行输入语句的insMethod

    static std::vector insPool = {ins("help", insHelp), ins("info", insInfo), ins("exit", insExit), ins("env", insEnv), ins("gen", insGen), ins("run", insRun), ins("", insEVAL)};

    // vm实例
    static yvm::YVM yovm;
    
    void runYoShell();
}