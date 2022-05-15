#include "yvm.h"
using namespace yvm::var;

// Space
Space::Space(){
    scopestack.push_back(Scope("YoGlobal"));
}

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

Space::Scope& Space::current() {return scopestack[deepcount];}

int Space::getScopePos(std::string name){
    int temp = deepcount;
    while(temp >= 0){
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
    if(scopestack[deepcount].idenname == "YoGlobal"); // 是全局Scope，不能删除
    else{
        deepcount --;
        scopestack.erase(scopestack.begin() + scopestack.size() - 1);
    }
}
Space::Scope Space::PopScope(std::string name){
    deepcount --;
    auto temp = scopestack[scopestack.size() - 1];
    scopestack.erase(scopestack.begin() + scopestack.size() - 1);
    return temp;
}

bool Space::find(std::string name){
    int temp = deepcount;
    while(temp >= 0){
        for(auto value: scopestack[temp].values){
            if(value.first == name) return true;
        }
        temp --;
    }
    return false;
}
Space::Scope::Value Space::getValue(std::string name){
    int temp = deepcount;
    while(temp >= 0){
        for(auto value: scopestack[temp].values){
            if(value.first == name) return value.second;
        }
        temp --;
    }
    throw yoexception::YoError("InsideError", "In: yvm::var::Space::Scope::getValue:Cannot find the value that name is '" + name + "'", -1, -1);
}
int Space::getValuePos(std::string name){
    int temp = deepcount;
    while(temp >= 0){
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
void Space::createValue(std::string name, Scope::Value value) {
    scopestack[deepcount].create(name, value);
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
void Space::Scope::create(std::string name, Value value){
    values.push_back(storage(name, value));
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


// Value      
Space::Scope::Value::Value(int val, bool isc, int ln, int col): 
                intvalue(val), isconst(isc), type(ygen::paraHelper::integer), line(ln), column(col) {}
Space::Scope::Value::Value(float val, bool isc, int ln, int col): 
                decivalue(val), isconst(isc), type(ygen::paraHelper::decimal), line(ln), column(col) {}
Space::Scope::Value::Value(std::string val, bool isc, int ln, int col): 
                strvalue(val), isconst(isc), type(ygen::paraHelper::string), line(ln), column(col) {}
Space::Scope::Value::Value(char val, bool isc, int ln, int col): 
                charvalue(val), isconst(isc), type(ygen::paraHelper::character), line(ln), column(col) {}
Space::Scope::Value::Value(bool val, bool isc, int ln, int col): 
                boolvalue(val), isconst(isc), type(ygen::paraHelper::boolean), line(ln), column(col) {}

Space::Scope::Value::Value(std::vector<Value> list, int ln, int col): 
                list(list), islist(true), type(list[0].getType()), line(ln), column(col) {}
             
Space::Scope::Value::Value(std::vector<Value> list, bool isc, int ln, int col): 
                list(list), isconst(isc), islist(true), type(list[0].getType()), line(ln), column(col) {}

bool Space::Scope::Value::isList(){
    return islist;
}
bool Space::Scope::Value::isConst(){
    return isconst;
}
ygen::paraHelper::type Space::Scope::Value::getType(){
    return type;
}

int Space::Scope::Value::getIntValue() {
    return intvalue;
}
float Space::Scope::Value::getDeciValue() {
    return decivalue;
}
std::string Space::Scope::Value::getStrValue() {
    return strvalue;
}
char Space::Scope::Value::getCharValue() {
    return charvalue;
}
bool Space::Scope::Value::getBoolValue() {
    return boolvalue;
}

template<class Type>
void Space::Scope::Value::assignValue(Type val){
    if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::integer) intvalue = val;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::decimal) decivalue = val;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::string) strvalue = val;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::character) charvalue = val;
    else if(std::is_same<typename std::decay<Type>::type, int>::value && type == ygen::paraHelper::boolean) boolvalue = val;
    else throw yoexception::YoError("TypeError", "Assignment type does not match variable type", line, column);
}

void Space::Scope::Value::assignValue(std::string name, std::vector<Value> value) {
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
yvm::YVM::YVM() {}

int yvm::YVM::addString(std::string str){
    constpool.push_back(str);
    return constpool.size()-1;
}
int yvm::YVM::addList(std::vector<vmValue> values){
    listpool.push_back(values);
    return listpool.size()-1;
}
int yvm::YVM::addChar(char ch){
    std::string str;
    str.push_back(ch);
    return constpool.size()-1;
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
    auto ret = runtimeTop == 0?runtimeStack[0]:runtimeStack[-- runtimeTop];
    runtimeStack.pop_back();
    return ret;
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
                if(codes[i].arg2 == vmVType::iden){
                    if(codes[i + 1].code == ygen::btc::idenend){
                        // 单个identifier，直接push
                        if(runtimeSpace.find(constpool[codes[i].arg1])){
                            auto type = (vmVType)runtimeSpace.getValue(constpool[codes[i].arg1]).getType();
                            envPush(vmValue(type, 
                                type == vmVType::integer?runtimeSpace.getValue(constpool[codes[i].arg1]).getIntValue():
                                    (type == vmVType::decimal?runtimeSpace.getValue(constpool[codes[i].arg1]).getDeciValue():
                                        (type == vmVType::boolean?runtimeSpace.getValue(constpool[codes[i].arg1]).getBoolValue():
                                            (type == vmVType::string?addString(runtimeSpace.getValue(constpool[codes[i].arg1]).getStrValue()):
                                                (type == vmVType::character?addChar(runtimeSpace.getValue(constpool[codes[i].arg1]).getCharValue()):
                                                    throw yoexception::YoError("TypeError","Unsupported type used",codes[i].line, codes[i].column))
                                                )
                                            )
                                        )
                                    )
                                );
                        }
                        else throw yoexception::YoError("NameError", "Cannot find identifier named: '" + constpool[codes[i].arg1] + "'", codes[i].line, codes[i].column);
                    }
                    else ; // 多个identifier连接在一起
                }
                else envPush(vmValue((vmVType)codes[i].arg2, codes[i].arg1));
                break;
            }
            case ygen::btc::listend:{
                envPush(vmValue(vmVType::flag, addString("LISTENDFLAG")));
                break;
            }
            case ygen::btc::lst:{
                std::vector<vmValue> list;
                bool flag = false;
                while(runtimeStack[runtimeTop].first != vmVType::flag){
                    list.push_back(envPop());
                }
                list.erase(list.begin() + list.size() - 1);
                std::reverse(list.begin(), list.end());
                envPush(vmValue(vmVType::list, addList(list)));
                break;
            }
            case ygen::btc::idx:{
                auto index = envPop();
                auto list = envPop();
                if(list.first == vmVType::list){
                    auto listnew = listpool[list.second];
                    envPush(vmValue(listnew[0].first, listnew[index.second].second));
                }   
                else throw yoexception::YoError("SyntaxError", "Cannot index an object that is not a composite object", codes[i].line, codes[i].column);
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
                break;
            }
            case ygen::btc::logicor:{
                auto right = envPop();
                auto left = envPop();
                envPush(vmValue(vmVType::boolean, (bool)left.second==true || (bool)right.second==true));
                break;
            }
            case ygen::btc::out:{
                auto content = envPop();
                if(content.first == vmVType::string || content.first == vmVType::character)
                    std::cout<<constpool[content.second]<<std::endl;
                else
                    std::cout<<content.second<<std::endl;
                break;
            }
            case ygen::btc::define:{
                if(runtimeSpace.find(constpool[codes[i].arg1])) // 检测是否有重复的标识符
                    throw yoexception::YoError("NameError", "Duplicate identifier: '" + constpool[codes[i].arg1] + "'", codes[i].line, codes[i].column);
                auto &current = runtimeSpace.current();
                current.memberlist.push_back(constpool[codes[i].arg1]);
                break;
            }            
            case ygen::btc::init:{
                auto name = constpool[codes[i].arg1];
                if(std::find(runtimeSpace.current().memberlist.begin(), runtimeSpace.current().memberlist.end(), name) != runtimeSpace.current().memberlist.end()){
                    auto value = envPop();
                    //type-checker
                    if(codes[i].arg4 == 1.0){
                        auto reqType = constpool[codes[i].arg3];
                        if(reqType == "integer" && value.first == vmVType::integer);
                        else if(reqType == "decimal" && value.first == vmVType::decimal);
                        else if(reqType == "string" && value.first == vmVType::string);
                        else if(reqType == "char" && value.first == vmVType::character);
                        else if(reqType == "boolean" && value.first == vmVType::boolean);
                        else {
                            // 不允许创建该变量，于是删除对应memberlist中的元素
                            auto &scope = runtimeSpace.current();
                            for(int i = 0; i < scope.memberlist.size(); i++) {
                                if(scope.memberlist[i] == name) 
                                    scope.memberlist.erase(scope.memberlist.begin() + i);
                            }
                            throw yoexception::YoError("TypeError", "The expected type does not match the type given by the actual expression", codes[i].line, codes[i].column);
                        }
                    }
                    //
                    if(value.first == vmVType::integer)
                        runtimeSpace.createValue(name, Space::Scope::Value((int)value.second, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                    else if(value.first == vmVType::decimal)
                        runtimeSpace.createValue(name, Space::Scope::Value((float)value.second, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                    else if(value.first == vmVType::boolean)
                        runtimeSpace.createValue(name, Space::Scope::Value((bool)value.second, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                    else if(value.first == vmVType::string)
                        runtimeSpace.createValue(name, Space::Scope::Value(constpool[value.second], constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                    else if(value.first == vmVType::character)
                        runtimeSpace.createValue(name, Space::Scope::Value(constpool[value.second][0], constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                    else if(value.first == vmVType::list) {
                        
                    }
                }
                break;
            }
            default: break;
        }
    }
    return 0;
}
void yvm::YVM::reload(std::vector<ygen::byteCode> _codes, std::vector<std::string> _constpool){
    codes = _codes;
    constpool = _constpool;
}