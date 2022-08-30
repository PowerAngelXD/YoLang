#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../exception/yoexc.h"

namespace yolexer{
    static std::vector<std::string> typeList =
            {"integer", "decimal", "char", "string", "boolean", "string[]", "decimal[]", "char[]", "integer[]", "boolean[]", "null"};
    static std::vector<std::string> bifList =
            {"print", "println", "input", "fread", "vmcode", "add_const", "substr", "system"};
    enum yoTokType{Integer, Decimal, Identifier, KeyWord, String, Symbol, Eof};

    struct yoToken{
        std::string content;
        yoTokType type;
        int line, column;

        std::string toString();
    };

    class Lexer{
        std::vector<yoToken> tokenGroup;
        std::string input; //! 规定：末尾必须有“\0”
    public:
        Lexer(std::string _input);
        void generate();
        std::vector<yoToken> getTokenGroup();
    };
}