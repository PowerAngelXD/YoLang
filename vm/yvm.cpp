#include "yvm.h"

yvm::YVM::YVM(ygen::ByteCodeGenerator bcg) {
    this->constpool = bcg.getConstPool();
    this->codes = bcg.getCodes();
}
int yvm::YVM::addString(std::string str){
    constpool.push_back(str);
    return constpool.size()-1;
}


std::string yvm::YVM::getVersion() {
    return version;
}
std::vector<std::string> yvm::YVM::getPool(){
    return constpool;
}
void yvm::YVM::envPush(yvm::YVM::vmValue val) {
    if (runtimeStack.size() <= runtimeTop) {
        for (int i = runtimeStack.size() ; i <= runtimeTop ; i ++)
            runtimeStack.emplace_back(vmValue(vmVType::null, 0.0));
    }
    runtimeStack[runtimeTop ++] = val;
}
yvm::YVM::vmValue yvm::YVM::envPop() {
    return runtimeTop == 0?runtimeStack[0]:runtimeStack[-- runtimeTop];
}
yvm::YVM::vmValue yvm::YVM::envPeek() {
    auto ret = runtimeTop == 0?runtimeStack[0]:runtimeStack[-- runtimeTop];
    runtimeTop ++;
    return ret;
}
void yvm::YVM::envClear(){
    runtimeStack.clear();
    runtimeTop = 0;
}

int yvm::YVM::run(std::string arg) {
    for(int i = 0; i < codes.size(); i++) {
        switch (codes[i].code)
        {
            case ygen::btc::push:{
                envPush(vmValue((vmVType)codes[i].arg2, codes[i].arg1));
                break;
            }
            case ygen::btc::add:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::integer, left.second + right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::decimal, left.second + right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::decimal, left.second + right.second));
                else if(left.first == vmVType::string && right.first == vmVType::string)
                    envPush(vmValue(vmVType::string, addString(constpool[left.second] + constpool[right.second])));
                else if(left.first == vmVType::character && right.first == vmVType::character)
                    envPush(vmValue(vmVType::string, addString(constpool[left.second] + constpool[right.second])));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::sub:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::integer, left.second - right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::decimal, left.second - right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::decimal, left.second - right.second));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::div:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::integer, left.second / right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::decimal, left.second / right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::decimal, left.second / right.second));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::mul:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::integer, left.second * right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::decimal, left.second * right.second));
                    else if(left.first == vmVType::string){
                        std::string newstr;
                        for(int i = 0; i < right.second; i++)
                            newstr += constpool[left.second];
                        envPush(vmValue(vmVType::string, addString(newstr)));
                    }
                    else if(right.first == vmVType::character){
                        std::string newstr;
                        for(int i = 0; i < left.second; i++)
                            newstr += constpool[right.second];
                        envPush(vmValue(vmVType::string, addString(newstr)));
                    }
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::decimal, left.second * right.second));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::tmo:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer && right.first == vmVType::integer)
                    envPush(vmValue(vmVType::integer, (int)left.second % (int)right.second));
                else 
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::equ:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::boolean, left.second == right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::boolean, left.second == right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::boolean, left.second == right.second));
                else if(left.first == vmVType::string && right.first == vmVType::string)
                    envPush(vmValue(vmVType::boolean, constpool[left.second] == constpool[right.second]));
                else if(left.first == vmVType::character && right.first == vmVType::character)
                    envPush(vmValue(vmVType::boolean, constpool[left.second] == constpool[right.second]));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::noequ:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::boolean, left.second != right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::boolean, left.second != right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::boolean, left.second != right.second));
                else if(left.first == vmVType::string && right.first == vmVType::string)
                    envPush(vmValue(vmVType::boolean, constpool[left.second] != constpool[right.second]));
                else if(left.first == vmVType::character && right.first == vmVType::character)
                    envPush(vmValue(vmVType::boolean, constpool[left.second] != constpool[right.second]));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::gt:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::boolean, left.second > right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::boolean, left.second > right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::boolean, left.second > right.second));
                else if(left.first == vmVType::string && right.first == vmVType::string)
                    envPush(vmValue(vmVType::boolean, constpool[left.second].size() > constpool[right.second].size()));
                else if(left.first == vmVType::character && right.first == vmVType::character)
                    envPush(vmValue(vmVType::boolean, constpool[left.second].size() > constpool[right.second].size()));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::gtet:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::boolean, left.second >= right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::boolean, left.second >= right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::boolean, left.second >= right.second));
                else if(left.first == vmVType::string && right.first == vmVType::string)
                    envPush(vmValue(vmVType::boolean, constpool[left.second].size() >= constpool[right.second].size()));
                else if(left.first == vmVType::character && right.first == vmVType::character)
                    envPush(vmValue(vmVType::boolean, constpool[left.second].size() >= constpool[right.second].size()));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::lt:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::boolean, left.second < right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::boolean, left.second < right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::boolean, left.second < right.second));
                else if(left.first == vmVType::string && right.first == vmVType::string)
                    envPush(vmValue(vmVType::boolean, constpool[left.second].size() < constpool[right.second].size()));
                else if(left.first == vmVType::character && right.first == vmVType::character)
                    envPush(vmValue(vmVType::boolean, constpool[left.second].size() < constpool[right.second].size()));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::ltet:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == vmVType::integer || right.first == vmVType::integer){
                    if(left.first == vmVType::integer && right.first == vmVType::integer)
                        envPush(vmValue(vmVType::boolean, left.second <= right.second));
                    else if(left.first == vmVType::decimal || right.first == vmVType::decimal)
                        envPush(vmValue(vmVType::boolean, left.second <= right.second));
                }
                else if(left.first == vmVType::decimal && right.first == vmVType::decimal)
                    envPush(vmValue(vmVType::boolean, left.second <= right.second));
                else if(left.first == vmVType::string && right.first == vmVType::string)
                    envPush(vmValue(vmVType::boolean, constpool[left.second].size() <= constpool[right.second].size()));
                else if(left.first == vmVType::character && right.first == vmVType::character)
                    envPush(vmValue(vmVType::boolean, constpool[left.second].size() <= constpool[right.second].size()));
                else
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",
                            codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::no:{
                envPush(vmValue(vmVType::boolean, envPop().second == true?0:1));
            }
            case ygen::btc::logicand:{
                auto right = envPop();
                auto left = envPop();
                envPush(vmValue(vmVType::boolean, (bool)left.second==true && (bool)right.second==true));
            }
            case ygen::btc::logicor:{
                auto right = envPop();
                auto left = envPop();
                envPush(vmValue(vmVType::boolean, (bool)left.second==true || (bool)right.second==true));
            }
            default: break;
        }
    }
    return 0;
}