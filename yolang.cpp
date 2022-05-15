#include <iostream>
#include "vm/yvm.h"

int main(){
    yvm::YVM vm;
    std::string cmd;
    std::cout<<"Yolang Shell [v0.0.1 (010)]"<<std::endl;
    while(true){
        try{
            std::cout<<">>";
            std::getline(std::cin, cmd);
            if(cmd == "exit") break;
            else{
                yolexer::Lexer lex(cmd); // 初始化lexer对象
                lex.generate(); // 生成TokenGroup
                parser::Parser p(lex.getTokenGroup()); // 初始化parser对象，传入tokenGroup
                auto node = p.parse(); // 开始parse
                //std::cout<<node->toString()<<std::endl;
                ygen::ByteCodeGenerator bcg(node);
                bcg.visit(node);
                vm = yvm::YVM(bcg);
                vm.run("null");
            }
        }
        catch(yoexception::YoError e){
            std::cout<<e.what()<<std::endl;
        }
    }
}