#include "yoshell.h"

// TOOLS
std::vector<std::string> ysh::tools::split(std::string str, char sp) {
    std::vector<std::string> ret;
    int i = 0;
    while(i < str.size()) {
        std::string content;
        for(; i < str.size(); i ++) {
            if(str[i] == sp) {i++; break;}
            content.push_back(str[i]);
        }
        ret.push_back(content);
    }
    return ret;
}
bool ysh::tools::compareFileType(std::string name, std::string type) {
    std::size_t index = ((std::string)name).find(type, ((std::string)name).size() - ((std::string)type).size());
    return !(index == std::string::npos);
}

// SHELL
void ysh::insHelp(std::vector<std::string> paras) {
    std::cout<<"Yoshell helper"<<std::endl;
    std::cout<<"help       view the help document"<<std::endl;
    std::cout<<"exit       exit the shell"<<std::endl;
    std::cout<<"info       view the information of the Yolang"<<std::endl;
}
void ysh::insExit(std::vector<std::string> paras) {
    exit(0);
}
void ysh::insInfo(std::vector<std::string> paras) {
    std::cout<<"Yolang complete version: " << ysh::completeVersion << std::endl;
    std::cout<<"Yolang version: " << ysh::version << std::endl;
    std::cout<<"Copyright (c) ITCDT Team."<< std::endl;
}
void ysh::insEVAL(std::vector<std::string> paras) {
    auto eval = paras[0];

    yolexer::Lexer lexer(eval);
    lexer.generate();
    auto tg = lexer.getTokenGroup();

    parser::Parser p(tg);
    auto stmts = p.parse();

    ygen::ByteCodeGenerator bcg(stmts);
    bcg.visit(stmts);

    yovm.reload(bcg.getCodes(), bcg.getConstPool());
    yovm.run("null");
}

void ysh::runYoShell() {
    // 输出欢迎页面
    std::cout<<"Welcome! This is YoLang's shell\n" 
                "We recommend that you enter \"help\" for help\n" 
                "[current version] "<<ysh::version<<std::endl;
    std::string in; // 输入内容
    auto pool = insPool;
    try{
        while(true) {
            std::cout<<">>";
            std::getline(std::cin, in);
            // command parser
            auto tempField = tools::split(in, ' ')[0];
            bool flag = false;
            for(int i = 0; i < ysh::insPool.size(); i++){
                if(tempField == ysh::insPool[i].first) {
                    auto paras = tools::split(in, ' '); // 创建参数列表
                    paras.erase(paras.begin()); // 删除第一个元素，因为他是命令解析符
                    ysh::insPool[i].second(paras);
                    flag = true;
                }
            }
            if(!flag) insEVAL({in});
            std::cout<<std::endl;
        }
    }
    catch(yoexception::YoError yoerr){

    }
}