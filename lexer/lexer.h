#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "../exception/yoexc.h"

namespace yolexer{
    static std::vector<std::string> typeList =
            {"integer", "decimal", "char", "string", "boolean", "string[]", "decimal[]", "char[]", "integer[]", "boolean[]", "null"};
    static std::vector<std::string> bifList =
            {"print", "println", "input", "fread", "vmcode", "add_const", "substr", "system", "fwrite", "ref", "length", "randint", "rand_deci", "randstr", "split"};
    static std::vector<std::string> yoKeyWords = {
            "const", "let", "typeof", "out", "repeat",
            "while", "for", "for_each", "return", "func",
            "string", "integer", "decimal", "boolean", "delete",
            "string[]", "integer[]", "decimal[]", "boolean[]",
            "as", "dynamic", "static", "null", "nameof", "new",
            "pack"
    };
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