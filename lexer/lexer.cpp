#include "lexer.h"

std::string yolexer::yoToken::toString(){
    std::string ret = "token: {" + content + ", ";
    switch (type)
    {
    case yolexer::yoTokType::Integer:
        ret += "Integer";
        break;
    case yolexer::yoTokType::Decimal:
        ret += "Decimal";
        break;
    case yolexer::yoTokType::String:
        ret += "String";
    break;
    case yolexer::yoTokType::Identifier:
        ret += "Identifier";
    break;
    case yolexer::yoTokType::KeyWord:
        ret += "KeyWord";
    break;
    case yolexer::yoTokType::Symbol:
        ret += "Symbol";
    break;
}
ret += ", " + std::to_string(line) + ", " + std::to_string(column) + "}";
return ret;
}


yolexer::Lexer::Lexer(std::string _input): input(_input) {}

std::vector<yolexer::yoToken> yolexer::Lexer::getTokenGroup() {return tokenGroup;}

/**
 * @brief 生成token
 *
 */
void yolexer::Lexer::generate(){
    int line = 1, column = 0;
    // yolang关键字表
    std::vector<std::string> yoKeyWords = {
        "var", "ref", "typeof", "const", "out", "repeat",
        "while", "for", "for_each", "return", "func", 
        "string", "integer", "decimal", "char", "boolean", "delete",
        "string[]", "integer[]", "decimal[]", "char[]", "boolean[]", "function",
        "as"
    }; // 新增关键字都在这里
    for(int i = 0; i < input.size(); i++){
        if(input[i] == '\n'){
            line ++; column = 0;
        }
        else column ++;

        if(input[i] == ' ' || input[i] == '\t' || input[i] == '\n');
        else if(isdigit(input[i])){
            //是数字
            std::string content;
            bool isDeci = false;
            while(isdigit(input[i]) || input[i] == '.'){
                if(input[i] == '.' && isDeci) 
                    throw yoexception::YoError("NumberError", "Extra decimal point", line, column);
                if(input[i] == '.') isDeci = true;
                content.push_back(input[i]);
                i++;column++;
            }
            i--;
            if(isDeci)
                tokenGroup.push_back({content, yoTokType::Decimal, line, column});
            else
                tokenGroup.push_back({content, yoTokType::Integer, line, column});
        }
        else if(input[i] == '"'){
            //是字符串
            std::string content;
            i++;column++; // 跳过字符 ”
            while(input[i] != '\"'){
                if(input[i] == '\0') throw yoexception::YoError("SyntaxError", "Expect '\"'!", line, column);
                content.push_back(input[i]);
                i++;column++;
            }
            std::string ret;
            // 对于转义符的处理
            for(int k = 0; k < content.size(); k ++) {
                if(content[k] == '\\') {
                    switch (content[k]) {
                        case '\\': break;
                        case 'b': ret += "\b"; break;
                        case 't': ret += "\t"; break;
                        case '0': ret += "\0"; break;
                        case 'a': ret += "\a"; break;
                        case 'f': ret += "\f"; break;
                        case 'n': ret += "\n"; break;
                        case 'r': ret += "\r"; break;
                        case '\"': ret += "\""; break;
                    }
                }
                ret += content[k];
            }
            //
            tokenGroup.push_back({content, yoTokType::String, line, column});
            end:
            ;
        }
        else if(isalpha(input[i]) || input[i] == '_' || input[i] == '$'){
            //是标识符
            bool isKeyWord = false;
            std::string content;
            while(isdigit(input[i]) || isalpha(input[i]) || input[i] == '_' || input[i] == '$'){
                content.push_back(input[i]);
                i++;column++;
            }
            i--;column--; // 防止跳过下一个字符
            if(input[i + 1] == '[' && input[i + 2] == ']') {
                i += 2;
                column += 2;
                content += "[]";
            }
            for(auto word: yoKeyWords){
                if(word == content){
                    tokenGroup.push_back({content, yoTokType::KeyWord, line, column});
                    isKeyWord = true;
                    continue;
                }
            }
            if(!isKeyWord)
                tokenGroup.push_back({content, yoTokType::Identifier, line, column});
        }
        else if(input[i] == '#'){
            while(input[i] != '\n' && input[i] != '\0')
                    i++;column++;
        }
        else {
            //是各种符号
            if(input[i] == '='){
                if(input[i + 1] == '='){
                    i++;
                    tokenGroup.push_back({"==", yoTokType::Symbol, line, column});
                    column++;
                }
                else if(input[i + 1] == '>'){
                    i++;
                    tokenGroup.push_back({"=>", yoTokType::Symbol, line, column});
                    column++;
                }
                else {
                    tokenGroup.push_back({"=", yoTokType::Symbol, line, column});
                }
            }
            else if(input[i] == '+'){
                if(input[i + 1] == '+'){
                    i++;
                    tokenGroup.push_back({"++", yoTokType::Symbol, line, column});
                    column++;
                }
                else
                    tokenGroup.push_back({"+", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '-'){
                if(input[i + 1] == '-'){
                    i++;
                    tokenGroup.push_back({"--", yoTokType::Symbol, line, column});
                    column++;
                }
                else if(input[i + 1] == '>'){
                    i++;
                    tokenGroup.push_back({"->", yoTokType::Symbol, line, column});
                    column++;
                }
                else 
                    tokenGroup.push_back({"-", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '>'){
                if(input[i + 1] == '='){
                    i++;
                    tokenGroup.push_back({">=", yoTokType::Symbol, line, column});
                    column++;
                }
                else 
                    tokenGroup.push_back({">", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '<'){
                if(input[i + 1] == '='){
                    i++;
                    tokenGroup.push_back({"<=", yoTokType::Symbol, line, column});
                    column++;
                }
                else 
                    tokenGroup.push_back({"<", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '|'){
                if(input[i + 1] == '|'){
                    i++;
                    tokenGroup.push_back({"||", yoTokType::Symbol, line, column});
                    column++;
                }
                else if(input[i + 1] == '>'){
                    i++;
                    tokenGroup.push_back({"|>", yoTokType::Symbol, line, column});
                    column++;
                }
                else
                    tokenGroup.push_back({"|", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '&'){
                if(input[i + 1] == '&'){
                    i++;
                    tokenGroup.push_back({"&&", yoTokType::Symbol, line, column});
                    column++;
                }
                else 
                    tokenGroup.push_back({"&", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '!'){
                if(input[i + 1] == '='){
                    i++;
                    tokenGroup.push_back({"!=", yoTokType::Symbol, line, column});
                    column++;
                }
                else
                    tokenGroup.push_back({"!", yoTokType::Symbol, line, column});
            }
            else if(input[i] == ':'){
                if(input[i + 1] == '=') {
                    i++;
                    tokenGroup.push_back({":=", yoTokType::Symbol, line, column});
                    column++;
                }
                else
                    tokenGroup.push_back({":", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '%') tokenGroup.push_back({"%", yoTokType::Symbol, line, column});
            else if(input[i] == '/') tokenGroup.push_back({"/", yoTokType::Symbol, line, column});
            else if(input[i] == '*') tokenGroup.push_back({"*", yoTokType::Symbol, line, column});
            else if(input[i] == '.') tokenGroup.push_back({".", yoTokType::Symbol, line, column});
            else if(input[i] == ',') tokenGroup.push_back({",", yoTokType::Symbol, line, column});
            else if(input[i] == '(') tokenGroup.push_back({"(", yoTokType::Symbol, line, column});
            else if(input[i] == ')') tokenGroup.push_back({")", yoTokType::Symbol, line, column});
            else if(input[i] == '[') tokenGroup.push_back({"[", yoTokType::Symbol, line, column});
            else if(input[i] == ']') tokenGroup.push_back({"]", yoTokType::Symbol, line, column});
            else if(input[i] == '{') tokenGroup.push_back({"{", yoTokType::Symbol, line, column});
            else if(input[i] == '}') tokenGroup.push_back({"}", yoTokType::Symbol, line, column});
            else if(input[i] == ';') tokenGroup.push_back({";", yoTokType::Symbol, line, column});
            else {
                std::string name;
                name.push_back(input[i]);
                throw yoexception::YoError("SyntaxError", "Invalid character: '" + name + "'!", line, column);
            }

        }
    }
    tokenGroup.push_back({"EOF", yoTokType::Eof, line, column});
}