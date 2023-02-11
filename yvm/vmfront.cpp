#include "vmfront.h"

std::vector<std::string> yvm::tools::split(std::string str, char sp) {
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
bool yvm::tools::compareFileType(std::string name, std::string type) {
    std::size_t index = ((std::string)name).find(type, ((std::string)name).size() - ((std::string)type).size());
    return !(index == std::string::npos);
}
std::string yvm::tools::stmtsToString(std::vector<AST::StmtNode*> stmts) {
    std::string ret = "Stmts: { [";
    for(auto stmt: stmts){
        ret += stmt->toString();
        ret += ", ";
    }
    ret += "] }";
    return ret;
}
std::string yvm::tools::mulStr(std::string str, int times) {
    std::string result;
    for(int i = 0; i < times; i++) {
        result += str;
    }
    return result;
}
std::string yvm::tools::formatAst(std::string aststr) {
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

//
void yvm::start(yshBase* yshObj) {
    yshObj->yshStart();
    try {
        while(true) {
            try {
                yshObj->yshUpdate();
            }
            catch(yoexception::YoError e){
                yshObj->catchYoError(e);
            }
        }
    }
    catch(yoexception::YshStopFlag flag){}
    catch(...){}
    yshObj->yshEnd();
}

//

yvm::vmfront &yvm::vmfront::newValue(std::string name, ysto::Value value) {
    vm.space.createValue(name, value);
    return *this;
}

yvm::vmfront &yvm::vmfront::clearVM() {
    vm = vmcore::vm();
    return *this;
}

yvm::vmfront &yvm::vmfront::resetVM(std::vector<byteCode> codes, std::vector<std::string> consts) {
    vm.load(consts, codes);
    return *this;
}

yvm::vmfront &yvm::vmfront::setupVM() {
    vm.space.createValue("Point", vm.native.bisSet.Point());
    vm.space.createValue("Application", vm.native.bisSet.Application());

    vm.space.createValue("yolang", vm.native.bisiSet.appFromApplication());

    vm.space.createValue("completeVersion", ysto::Value(ytype::YString(compeleteVersion), true, 0, 0));
    vm.space.createValue("releaseVersion", ysto::Value(ytype::YString(releaseVersion), true, 0, 0));
    vm.space.createValue("_integer", ysto::Value(ytype::YString("Integer"), true, 0, 0));
    vm.space.createValue("_decimal", ysto::Value(ytype::YString("Decimal"), true, 0, 0));
    vm.space.createValue("_string", ysto::Value(ytype::YString("String"), true, 0, 0));
    vm.space.createValue("_boolean", ysto::Value(ytype::YString("Boolean"), true, 0, 0));

    for(auto fn: yolexer::bifList) {
        vm.space.createValue(fn, ysto::Value(ytype::YObject({}, {}, {}, fn), true, 0, 0));
    }

    return *this;
}

yvm::vmfront &yvm::vmfront::runVM(std::string arg) {
    vm.run(-1, arg);
    return *this;
}

vmcore::vm yvm::vmfront::getVM() {
    return vm;
}

//

std::vector<yolexer::yoToken> yvm::yshBase::makeTokenGroup(std::string text) {
    yolexer::Lexer lex(text);
    lex.generate();
    return lex.getTokenGroup();
}

void yvm::yshBase::yshStart() {}

void yvm::yshBase::yshUpdate() {}

void yvm::yshBase::yshEnd() {}

void yvm::yshBase::catchYoError(yoexception::YoError ye) {}

yvm::yshBase::yshBase() {}

//

void yvm::InsSet::repl(yvm::ins_method_para para, yshBase* ysh) {}

void yvm::InsSet::help(yvm::ins_method_para para, yshBase* ysh) {}

void yvm::InsSet::info(yvm::ins_method_para para, yshBase* ysh) {}

void yvm::InsSet::exit(yvm::ins_method_para para, yshBase* ysh) {}
