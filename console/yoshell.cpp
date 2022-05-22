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
    std::cout<<"Yo-Console Helper"<<std::endl;
    std::cout<<"help....................view the help document"<<std::endl;
    std::cout<<"exit....................exit the shell"<<std::endl;
    std::cout<<"run (file)..............run a yolang source file or VM intermediate code file"<<std::endl;
    std::cout<<"gen (file) (target).....generate a virtual machine file named 'target' from the file named 'file'"<<std::endl;
    std::cout<<"env (opt).....          operate on virtual machines"<<std::endl;
    std::cout<<"info....................view the information of the Yolang"<<std::endl;
}
void ysh::insExit(std::vector<std::string> paras) {
    exit(0);
}
void ysh::insInfo(std::vector<std::string> paras) {
    std::cout<<"Yolang develop complete version: " << ysh::completeVersion << std::endl;
    std::cout<<"Yolang develop version: " << ysh::version << std::endl;
    std::cout<<"Yolang version: " << ysh::relVersion << std::endl;
    std::cout<<"Copyright (c) ITCDT Team."<< std::endl;
}
void ysh::insRun(std::vector<std::string> paras) {
    if(paras.size() > 1)
        throw yoexception::YoError("ConsoleParaError", "Too many parameters", -1, -1);
    if(paras.size() < 1)
        throw yoexception::YoError("ConsoleParaError", "Too few parameters", -1, -1);
    else {
        auto name = paras[0];
        if(tools::compareFileType(name, ".yvmc")) {
            // 是vm文件
            yovm.loadVMFile(name);
            yovm.run("null");
        }
        else if(tools::compareFileType(name, ".yo")) {
            // 是源文件
            std::ifstream file(name);
            std::istreambuf_iterator<char> begin(file);
            std::istreambuf_iterator<char> end;
            std::string str(begin, end);

            yolexer::Lexer lexer(str);
            lexer.generate();
            auto tg = lexer.getTokenGroup();

            parser::Parser p(tg);
            auto stmts = p.parse();

            ygen::ByteCodeGenerator bcg(stmts);
            bcg.visit(stmts);
        
            yovm.reload(bcg.getCodes(), bcg.getConstPool());
            yovm.run("null");
        }
    }
}
void ysh::insGen(std::vector<std::string> paras) {
    if(paras.size() > 2)
        throw yoexception::YoError("ConsoleParaError", "Too many parameters", -1, -1);
    if(paras.size() < 2)
        throw yoexception::YoError("ConsoleParaError", "Too few parameters", -1, -1);
    auto targetName = paras[1];
    auto name = paras[0];

    std::ifstream file(name);
    std::istreambuf_iterator<char> begin(file);
    std::istreambuf_iterator<char> end;
    std::string str(begin, end);

    yolexer::Lexer lexer(str);
    lexer.generate();
    auto tg = lexer.getTokenGroup();

    parser::Parser p(tg);
    auto stmts = p.parse();

    ygen::ByteCodeGenerator bcg(stmts);
    bcg.visit(stmts);
    bcg.genFile(targetName);
    std::cout<<"File generation completed! File name: '"<<targetName<<"'";

}
void ysh::insEVAL(std::vector<std::string> paras) {
    auto eval = paras[0];

    yolexer::Lexer lexer(eval);
    lexer.generate();
    auto tg = lexer.getTokenGroup();

    parser::Parser p(tg);
    if(tg[tg.size() - 2].content != ";" && tg[tg.size() - 2].content != "}") {
        auto expr = p.parseExpr();

        ygen::ByteCodeGenerator bcg(expr);
        bcg.visitExpr(expr);

        yovm.reload(bcg.getCodes(), bcg.getConstPool());
        yovm.run("null");

        if(yovm.envPeek().first == yovm.string || yovm.envPeek().first == yovm.character)
            std::cout<<yovm.getConstPool()[yovm.envPop().second];
        else if(yovm.envPeek().first == yovm.null)
            std::cout<<"null";
        else if(yovm.envPeek().first == yovm.list) {
            auto list = yovm.getListPool()[yovm.envPop().second];
            std::cout<<"[";
            for(int i = 0; i < list.size(); i ++){
                auto elt = list[i];
                if(elt.first == yovm.string || elt.first == yovm.character) {
                    std::cout<<"\"";
                    std::cout<<yovm.getConstPool()[elt.second];
                    std::cout<<"\"";
                }
                else if(elt.first == yovm.null)
                    std::cout<<"null";
                else
                    std::cout<<elt.second;
                if(i != list.size() - 1) std::cout<<", ";
            }
            std::cout<<"]";
        }
        else
            std::cout<<yovm.envPop().second;
    }
    else {
        auto stmts = p.parse();

        ygen::ByteCodeGenerator bcg(stmts);
        bcg.visit(stmts);
        
        yovm.reload(bcg.getCodes(), bcg.getConstPool());
        yovm.run("null");
    }
}
void ysh::insEnv(std::vector<std::string> paras) {
    if(paras.empty()) std::cout<<"YVM (Yolang Virtual Machine)";
    else {
        if(paras.size() == 1) {
            if(paras[0] == "clear") {
                yovm.clear();
                std::cout<<"[YVM SUCCESS] Operation execution completed";
            }
            else std::cout<<"[YVM ERROR] Unknown operation on virtual machine: '" + paras[0] + "'";
        }
    }
}

void ysh::runYoShell() {
    // 输出欢迎页面
    std::cout<<"Yolang "<<ysh::completeVersion<<"("<<ysh::version<<")\nGCC version: 8.1.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project)"<<" \nWe recommend that you enter \"help\" for help"<<std::endl;
    std::string in; // 输入内容
    auto pool = insPool;

    while(true) {
        std::cout<<">>";
        std::getline(std::cin, in);
        try{
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
        }
        catch(yoexception::YoError e) {
            std::cout<<e.what();
        }
        std::cout<<std::endl;
    }
}