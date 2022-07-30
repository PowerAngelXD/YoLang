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
std::string ysh::tools::stmtsToString(std::vector<AST::StmtNode*> stmts) {
    std::string ret = "Stmts: { [";
    for(auto stmt: stmts){
        ret += stmt->toString();
        ret += ", ";
    }
    ret += "] }";
    return ret;
}
std::string ysh::tools::mulStr(std::string str, int times) {
    std::string result;
    for(int i = 0; i < times; i++) {
        result += str;
    }
    return result;
}
std::string ysh::tools::formatAst(std::string aststr) {
    /* Exaple:
     * node: {
     *      node1: {
     *          node2: {
     *              token: XXXXX
     *          }
     *      }
     * }
     * */
    std::string result;
    int bigparac = 0; // 大括号出现的计数器
    for(int i = 0; i < aststr.size(); i ++) {
        char ch = aststr[i];
        if(ch == '{') {
            result += ch;
            bigparac ++;
            result += "\n" + mulStr(" ", 4 * bigparac);
        }
        else if(ch == '}') {
            bigparac --;
            result += "\n" + mulStr(" ", 4 * bigparac);
            result += ch;
        }
        if(ch == '[') {
            result += ch;
            bigparac ++;
            result += "\n" + mulStr(" ", 4 * bigparac);
        }
        else if(ch == ']') {
            bigparac --;
            result += "\n" + mulStr(" ", 4 * bigparac);
            result += ch;
        }
        else result += ch;
    }
    return result;
}

// SHELL
void ysh::insHelp(std::vector<std::string> paras) {
    std::cout<<"Yo-Console Helper"<<std::endl;
    std::cout<<"help........................view the help document"<<std::endl;
    std::cout<<"exit........................exit the shell"<<std::endl;
    std::cout<<"run (file)..................run a yolang source file or VM intermediate code file"<<std::endl;
    std::cout<<"gen (file) (target).........generate a virtual machine file named 'target' from the file named 'file'"<<std::endl;
    std::cout<<"view (opt) (ins) ...........View the content of \"ins\" in the \"opt\" mode (opt refer to: \"ast\", \"file\")"<<std::endl;
    std::cout<<"env (opt)...................operate on virtual machines"<<std::endl;
    std::cout<<"info........................view the information of the Yolang"<<std::endl;
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
        }
        else if(tools::compareFileType(name, ".yo")) {
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

        }
        else
            throw yoexception::YoError("FileError", "Not any file that can be run", -1, -1);
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
void ysh::insView(std::vector<std::string> paras) {
    std::string key = paras[0];
    // 处理第一个参数后面的所有参数，全部连在一起，以防出错
    paras.erase(paras.begin());
    std::string text;
    for(auto str: paras)
        text += str + " ";
    //
    if(paras.size() >= 1) {
        if(key == "ast" || key == "a") {
            yolexer::Lexer lexer(text);
            lexer.generate();
            auto tg = lexer.getTokenGroup();
            parser::Parser p(tg);
            auto stmts = p.parse();
            std::cout<<"View content(AST): "<<std::endl;
            std::cout<<tools::stmtsToString(stmts)<<std::endl;
        }
        else if(key == "token" || key == "t") {
            yolexer::Lexer lexer(text);
            lexer.generate();
            auto tg = lexer.getTokenGroup();
            std::cout<<"View content(Token group): "<<std::endl;
            for(auto token: tg) {
                std::cout<<token.toString() + ", ";
            }
        }
        else if(key == "file" || key == "f") {
            std::ifstream file(paras[0]);
            if(!file)
                throw yoexception::YoError("FileError", "'" + paras[0] + "' is not a valid file name", -1, -1);
            std::istreambuf_iterator<char> begin(file);
            std::istreambuf_iterator<char> end;
            std::string content(begin, end);
            std::cout<<"View content(FILE): "<<std::endl;
            std::cout<<content<<std::endl;
        }
        else
            throw yoexception::YoError("ConsoleParaError", "'" + key + "' is not the correct parameter for this instruction", -1, -1);
    }
    else throw yoexception::YoError("ConsoleParaError", "Too many or few parameters", -1, -1);
}
void ysh::insEVAL(std::vector<std::string> paras) {
    auto eval = paras[0];

    yolexer::Lexer lexer(eval);
    lexer.generate();
    auto tg = lexer.getTokenGroup();

    parser::Parser p(tg);
    if(tg[tg.size() - 2].content != ";" && tg[tg.size() - 2].content != "}" && !(p.isStmt() && !p.isSpExprStmt())) {
        auto expr = p.parseExpr();

        ygen::ByteCodeGenerator bcg(expr);
        bcg.visitExpr(expr);

        yvm = vmcore::vm(bcg.getCodes(), bcg.getConstPool());
        yvm.run("null");

        std::cout<<yvm.getResult().getIntegerValue().get()<<std::endl;
        // TODO：返回值输出
    }
    else {
        auto stmts = p.parse();

        ygen::ByteCodeGenerator bcg(stmts);
        bcg.visit(stmts);

    }
}
void ysh::insEnv(std::vector<std::string> paras) {
    if(paras.empty()) std::cout<<"YVM (Yolang Virtual Machine)";
    else {
        if(paras.size() == 1) {
            if(paras[0] == "clear") {
                // clear
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
                    ysh::insPool[i].second(paras); // 执行对应的method
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