#include <iostream>
#include "lexer/lexer.h"
#include "parser/parser.h"

int main(){
    std::string cmd;
    while(true){
        try{
            std::cout<<">>";
            std::getline(std::cin, cmd);
            if(cmd == "exit") break;
            else{
                yolexer::Lexer lex(cmd); // 初始化lexer对象
                lex.generate(); // 生成TokenGroup
                parser::Parser p(lex.getTokenGroup()); // 初始化parser对象，传入tokenGroup
                auto node = p.parseExpr(); // 开始parse
                std::cout<<node->toString()<<std::endl;
            }
        }
        catch(yoexception::YoError e){
            std::cout<<e.what()<<std::endl;
        }
    }
}