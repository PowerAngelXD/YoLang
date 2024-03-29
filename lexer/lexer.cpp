#include "lexer.h"
using std::string;

std::string yolexer::yoToken::toString(){
    std::string ret = "token: {" + content + ", ";
    switch (type)
    {
        case yoTokType::Integer:
            ret += "Integer";
            break;
        case yoTokType::Decimal:
            ret += "Decimal";
            break;
        case yoTokType::String:
            ret += "String";
            break;
        case yoTokType::Identifier:
            ret += "Identifier";
            break;
        case yoTokType::KeyWord:
            ret += "KeyWord";
            break;
        case yoTokType::Symbol:
            ret += "Symbol";
            break;
        default:
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
    for(int i = 0; i < input.size(); i++){
        if(input[i] == '\n'){
            line ++; column = 0;
        }
        else column ++;

        if(input[i] == ' ' || input[i] == '\t' || input[i] == '\n');
        else if(input[i] == '/' && input[i + 1] == '/') {
            // 注释判断
            while(input[i] != '\n' && input[i] != '\0') {
                i++;
                column++;
            }
            line++;
            i++;
        }
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
            while(input[i] != '\"' || input[i - 1] == '\\'){
                if(input[i] == '\0') throw yoexception::YoError("SyntaxError", "Expect '\"'!", line, column);
                else if(input[i] == '\\') {
                    i++;
                    switch (input[i]) {
                        case '\\': content.push_back('\\'); break;
                        case 'b': content.push_back('\b'); break;
                        case 't': content.push_back('\t'); break;
                        case '0': content.push_back('\0'); break;
                        case 'a': content.push_back('\a'); break;
                        case 'f': content.push_back('\f'); break;
                        case 'n': content.push_back('\n'); break;
                        case 'r': content.push_back('\r'); break;
                        case '\"': content.push_back('\"'); break;
                    }
                }
                else content.push_back(input[i]);
                i++;column++;
            }
            std::string ret;
            tokenGroup.push_back({content, yoTokType::String, line, column});
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
            if(std::find(yoKeyWords.begin(), yoKeyWords.end(),content) != yoKeyWords.end()){
                tokenGroup.push_back({content, yoTokType::KeyWord, line, column});
            }
            else
                tokenGroup.push_back({content, yoTokType::Identifier, line, column});
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
                else if(input[i + 1] == '='){
                    i++;
                    tokenGroup.push_back({"+=", yoTokType::Symbol, line, column});
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
                else if(input[i + 1] == '='){
                    i++;
                    tokenGroup.push_back({"-=", yoTokType::Symbol, line, column});
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
                    if(input[i + 2] == '>') {
                        i+=2;
                        tokenGroup.push_back({"<=>", yoTokType::Symbol, line, column});
                        column++;
                    }
                    else {
                        i++;
                        tokenGroup.push_back({"<=", yoTokType::Symbol, line, column});
                        column++;
                    }
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
            else if(input[i] == '*') {
                if(input[i + 1] == '=') {
                    i++;
                    tokenGroup.push_back({"*=", yoTokType::Symbol, line, column});
                    column++;
                }
                else
                    tokenGroup.push_back({"*", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '/') {
                if(input[i + 1] == '=') {
                    i++;
                    tokenGroup.push_back({"/=", yoTokType::Symbol, line, column});
                    column++;
                }
                else
                    tokenGroup.push_back({"/", yoTokType::Symbol, line, column});
            }
            else if(input[i] == '%') tokenGroup.push_back({"%", yoTokType::Symbol, line, column});
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
