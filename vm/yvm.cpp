#include "yvm.h"
using namespace yvm::var;

// Space
Space::Scope Space::getScope(std::string name){
    int temp = deepcount;
    while(temp > 0){
        for(auto scope: scopestack){
            if(scope.findV(name)) return scope;
        }
        temp --;
    }
    throw yoexception::YoError("InsideError", "In: yvm::var::Space::getTargetScope:Cannot find the scope that name is '" + name + "'", -1, -1);
}
int Space::getScopePos(std::string name){
    int temp = deepcount;
    while(temp > 0){
        for(int i = 0; i < scopestack.size(); i++){
            if(scopestack[i].findV(name)) return i;
        }
        temp --;
    }
    return -1;
}
void Space::createScope(std::string name){
    deepcount ++;
    scopestack.push_back(Scope(name));
}
void Space::removeScope(){
    deepcount --;
    scopestack.erase(scopestack.begin() + scopestack.size() - 1);
}
Space::Scope Space::PopScope(std::string name){
    deepcount --;
    auto temp = scopestack[scopestack.size() - 1];
    scopestack.erase(scopestack.begin() + scopestack.size() - 1);
    return temp;
}

bool Space::find(std::string name){
    int temp = deepcount;
    while(temp > 0){
        for(auto value: scopestack[temp].values){
            if(value.first == name) return true;
        }
        temp --;
    }
    return false;
}
Space::Scope::Value Space::getValue(std::string name){
    int temp = deepcount;
    while(temp > 0){
        for(auto value: scopestack[temp].values){
            if(value.first == name) return value.second;
        }
        temp --;
    }
    throw yoexception::YoError("InsideError", "In: yvm::var::Space::Scope::getValue:Cannot find the value that name is '" + name + "'", -1, -1);
}
int Space::getValuePos(std::string name){
    int temp = deepcount;
    while(temp > 0){
        for(int i = 0; i < scopestack[temp].values.size(); i++){
            if(scopestack[temp].findV(name)) return i;
        }
        temp --;
    }
    return -1;
}
template<class Type>
void Space::assignValue(std::string name, Type value){
    scopestack[getScopePos(name)].assign(value);
}
template<class Type>
void Space::assignValue(std::string name, std::vector<Type> value){
    scopestack[getScopePos(name)].assign(value);
}
void Space::deleteValue(std::string name) {
    scopestack[getScopePos(name)].remove(name);
}
int Space::getDeep(){
    return deepcount;
}


// Scope
Space::Scope::Scope(std::string name): idenname(name) {}

bool Space::Scope::findV(std::string name){
    for(auto value: values){
        if(value.first == name) return true;
    }
    return false;
}
Space::Scope::Value Space::Scope::val(std::string name){
    for(auto value: values){
        if(value.first == name) value;
    }
    throw yoexception::YoError("InsideError", "In: yvm::var::Space::Scope::getValue:Cannot find the value that name is '" + name + "'", -1, -1);
}

template<class Type>
void Space::Scope::assign(std::string name, Type value){
    values[pos(name)].second.assignValue(value);
}
int Space::Scope::pos(std::string name){
    for(int i = 0; i < values.size(); i ++){
        if(values[i].first == name) return i;
    }
    return -1;
}
void Space::Scope::remove(std::string name) {
    if(findV(name)){
        values.erase(values.begin() + pos(name));
    }
    else throw yoexception::YoError("NameError", "Identifier with '" + name + "' does not exist", -1, -1);
}

std::string Space::Scope::getName(){
    return idenname;
}


// Value      
yvm::var::Space::Scope::Value::Value(int val, bool isc, int ln, int col): 
                intvalue(val), isconst(isc), type(ygen::paraHelper::integer), line(ln), column(col) {}
yvm::var::Space::Scope::Value::Value(float val, bool isc, int ln, int col): 
                decivalue(val), isconst(isc), type(ygen::paraHelper::decimal), line(ln), column(col) {}
yvm::var::Space::Scope::Value::Value(std::string val, bool isc, int ln, int col): 
                strvalue(val), isconst(isc), type(ygen::paraHelper::string), line(ln), column(col) {}
yvm::var::Space::Scope::Value::Value(char val, bool isc, int ln, int col): 
                charvalue(val), isconst(isc), type(ygen::paraHelper::character), line(ln), column(col) {}
yvm::var::Space::Scope::Value::Value(bool val, bool isc, int ln, int col): 
                boolvalue(val), isconst(isc), type(ygen::paraHelper::boolean), line(ln), column(col) {}

yvm::var::Space::Scope::Value::Value(std::vector<Value> list): list(list), islist(true), type(list[0].getType()) {}
             
yvm::var::Space::Scope::Value::Value(std::vector<Value> list, bool isc): list(list), isconst(isc), islist(true), type(list[0].getType()) {}

bool yvm::var::Space::Scope::Value::isList(){
    return islist;
}
bool yvm::var::Space::Scope::Value::isConst(){
    return isconst;
}
ygen::paraHelper::type yvm::var::Space::Scope::Value::getType(){
    return type;
}

template<class Type>
Type yvm::var::Space::Scope::Value::getValue(){
    if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::integer) return intvalue;
    else if(std::is_same<typename std::decay<Type>::type, int>::valuet && type == ygen::paraHelper::decimal) return decivalue;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::string) return strvalue;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::character) return charvalue;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::boolean) return boolvalue;
    else throw yoexception::YoError("TypeError", "There is no corresponding type for this operation", line, column);
}

template<class Type>
void yvm::var::Space::Scope::Value::assignValue(Type val){
    if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::integer) intvalue = val;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::decimal) decivalue = val;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::string) strvalue = val;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::character) charvalue = val;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::boolean) boolvalue = val;
    else throw yoexception::YoError("TypeError", "Assignment type does not match variable type", line, column);
}

void yvm::var::Space::Scope::Value::assignValue(std::string name, std::vector<Value> value) {
    if(value[0].getType() == type){
        list = value;
    }
    else throw yoexception::YoError("TypeError", "Assignment type does not match variable type", line, column);
}

// VM

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
                if(right.second == 0)
                    throw yoexception::YoError("CalculationError", "Divisor cannot be 0", codes[i].line, codes[i].column);
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
                    else if(left.first == vmVType::string || right.first == vmVType::string){
                        std::string newstr;
                        for(int i = 0; i < right.second; i++)
                            newstr += constpool[left.second];
                        envPush(vmValue(vmVType::string, addString(newstr)));
                    }
                    else if(left.first == vmVType::character || right.first == vmVType::character){
                        std::string newstr;
                        for(int i = 0; i < right.second; i++)
                            newstr += constpool[left.second];
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