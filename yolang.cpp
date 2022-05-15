#include <iostream>
#include "vm/yvm.h"

int main(){
    yvm::YVM vm;
    std::string cmd;
    std::cout<<"Yolang Shell [Instable-2022-5-0001]"<<std::endl;
    while(true){
        try{
            std::cout<<">>";
            std::getline(std::cin, cmd);
            if(cmd == "exit") break;
            else{
                yolexer::Lexer lex(cmd); // new lexer object
                lex.generate(); // make TokenGroup
                parser::Parser p(lex.getTokenGroup()); // new parser object，enter tokenGroup
                auto node = p.parse(); // start parse
                ygen::ByteCodeGenerator bcg(node);
                bcg.visit(node);
                vm.reload(bcg.getCodes(), bcg.getConstPool());
                vm.run("null");
            }
        }
        catch(yoexception::YoError e){
            std::cout<<e.what()<<std::endl;
        }
    }
}