#include "ysh.h"

void ysh_offical::YolangShellOffical::yshStart() {
    front.setupVM(); // 初始化vm

    std::cout<<"Yolang Shell  (version: "<<shellVersion<<")"<<std::endl;
    std::cout<<"Yolang Version: "<<front.compeleteVersion<<"("<<front.version<<")";
#ifdef __GNUC__
    std::cout<< "\nCompiled by gcc (author's version = 12.2.0)"<<std::endl;
#elif _MSC_VER
    std::cout<< "\nCompiled by MSVC"<<std::endl;
#endif
    std::cout<<"We suggest you enter \"help\" for help"<<std::endl;
}

void ysh_offical::YolangShellOffical::yshUpdate() {
    std::string text;

    std::cout<<">> ";
    std::getline(std::cin, text);

    if(text=="\0"||text=="\n") throw yoexception::YoError("InputError","Please do not enter invalid characters",1,1);
    auto tempField = yvm::tools::split(text, ' ')[0];

    auto para = yvm::tools::split(text, ' ');
    para.erase(para.begin());
    if(tempField == "help") ysi->help(para, this);
    else if(tempField == "info") ysi->info(para, this);
    else if(tempField == "run") ysi->run(para, this);
    else if(tempField == "view") ysi->view(para, this);
    else if(tempField == "exit") ysi->exit(para, this);
    else ysi->repl({text}, this);

    std::cout<<std::endl;
}

void ysh_offical::YolangShellOffical::yshEnd() {

}

void ysh_offical::YolangShellOffical::catchYoError(yoexception::YoError ye) {
    std::cout<<ye.what()<<std::endl;
}

ysh_offical::YolangShellOffical::YolangShellOffical() {

}

// ins set

void ysh_offical::YolangShellInsSet::repl(yvm::ins_method_para para, YolangShellOffical* yso) {
    auto repl_in = para[0];
    auto tg = yso->makeTokenGroup(repl_in);
    // 对是表达式还是语句进行判断
    try {
        yso->yparser = parser::Parser(tg);
        auto stmts = yso->yparser.parse(); // parse语句，如果报错八成就是表达式
        ygen::ByteCodeGenerator bcg;
        bcg.visit(stmts);
        yso->front.resetVM(bcg.getCodes(), bcg.getConstPool()).runVM("normal");
    }
    catch(yoexception::YoError ye){
        yso->yparser = parser::Parser(tg);
        auto expr = yso->yparser.parseExpr();
        ygen::ByteCodeGenerator bcg;
        bcg.visitExpr(expr);
        yso->front.resetVM(bcg.getCodes(), bcg.getConstPool()).runVM("repl");
        auto r = yso->front.getVM().getResult();
        std::cout<<"(return) ";
        ysto::printValue(r);
    }
}

void ysh_offical::YolangShellInsSet::help(yvm::ins_method_para para, YolangShellOffical* yso) {
    std::string help_text = "Yolang Shell Help-document"
                            "\nYou can use the following commands to operate in the shell:"
                            "\ninfo                          View information about Yolang and YoShell"
                            "\nrun [file_name(with '.yo')]   Run a Yolang-File"
                            "\nexit                          Exit the program"
                            "Tip: When the content you enter is not any of the above commands, YoShell will switch to REPL mode to parse the content you enter (according to Yolang syntax)";
    std::cout<<help_text<<std::endl;
}

void ysh_offical::YolangShellInsSet::info(yvm::ins_method_para para, YolangShellOffical* yso) {
    std::cout<<std::endl;
    std::cout<<"Yolang Shell (New)"<<std::endl;
    std::cout<<"Shell Version: "<<yso->shellVersion<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Yolang Detail:"<<std::endl;
    std::cout<<"Version: "<<yso->front.compeleteVersion<<"("<<yso->front.version<<")"<<std::endl;
    std::cout<<"Release Version: "<<yso->front.releaseVersion<<std::endl;
}

void ysh_offical::YolangShellInsSet::run(yvm::ins_method_para para, YolangShellOffical* yso) {
    auto name = para[0];
    if(yvm::tools::compareFileType(name, ".yo")){
        std::ifstream file(name);
        std::istreambuf_iterator<char> begin(file);
        std::istreambuf_iterator<char> end;
        std::string str(begin, end);


        auto tg = yso->makeTokenGroup(str);
        yso->yparser = parser::Parser(tg);
        auto stmts = yso->yparser.parse();
        ygen::ByteCodeGenerator bcg;
        bcg.visit(stmts);
        yso->front.resetVM(bcg.getCodes(), bcg.getConstPool());
        yso->front.setupVM().runVM("program").clearVM().setupVM();
    }
    else
        throw yoexception::YoError("FileError", "Not any file that can be run", -1, -1);
}

void ysh_offical::YolangShellInsSet::view(yvm::ins_method_para para, YolangShellOffical* yso) {

}

void ysh_offical::YolangShellInsSet::exit(yvm::ins_method_para para, YolangShellOffical* yso) {
    throw yoexception::YshStopFlag();
}
