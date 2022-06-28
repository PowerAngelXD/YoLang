#include "yvm.h"
using namespace yvm::var;

// Space
Space::Space(){
    scopestack.push_back(Scope("__global__"));
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
    if(scopestack[deepcount].idenname == "__global__"); // 是全局Scope，不能删除
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
Space::Scope::Value* Space::getValueRef(std::string name) {
    int temp = deepcount;
    while(temp >= 0){
        for(auto value: scopestack[temp].values){
            if(value.first == name) {
                return value.second.getRef();
            }
        }
        temp --;
    }
    throw yoexception::YoError("InsideError", "In: yvm::var::Space::Scope::getValue:Cannot find the value that name is '" + name + "'", -1, -1);
}
Space::Scope::Value* Space::getValueSelfRef(std::string name) {
    int temp = deepcount;
    while(temp >= 0){
        for(auto value: scopestack[temp].values){
            if(value.first == name) {
                auto ref = new Space::Scope::Value;
                ref = &value.second;
                return ref;
            }
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
    scopestack[getScopePos(name)].assign(name, value);
}
template<class Type>
void Space::assignValue(std::string name, std::vector<Type> value){
    scopestack[getScopePos(name)].assign(name, value);
}
void Space::deleteValue(std::string name) {
    scopestack[getScopePos(name)].remove(name);
}
void Space::createValue(std::string name, Scope::Value value) {
    scopestack[deepcount].create(name, value);
}
void Space::rmValueNull(std::string name) {
    scopestack[getValuePos(name)].rmNull(name);
}
void Space::reWriteType(std::string name, ygen::paraHelper::type t){
    scopestack[getValuePos(name)].rewriteType(name, t);
}
void Space::toggleList(std::string name) {
    scopestack[getValuePos(name)].changeList(name);
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

void Space::Scope::assign(std::string name, int value) {
    values[pos(name)].second.assignInt(value);
}
void Space::Scope::assign(std::string name, float value) {
    values[pos(name)].second.assignDeci(value);
}
void Space::Scope::assign(std::string name, bool value) {
    values[pos(name)].second.assignBool(value);
}
void Space::Scope::assign(std::string name, Object value) {
    values[pos(name)].second.assignObject(value);
}
void Space::Scope::assign(std::string name, char value) {
    values[pos(name)].second.assignChar(value);
}
void Space::Scope::assign(std::string name, std::string value) {
    values[pos(name)].second.assignString(value);
}
void Space::Scope::assign(std::string name, Value value) {
    values[pos(name)].second.refAnother(value);
}
void Space::Scope::assign(std::string name, std::vector<Value> value) {
    values[pos(name)].second.assignListValue(value);
}
void Space::Scope::rewriteType(std::string name, ygen::paraHelper::type t) {
    values[pos(name)].second.type = t;
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
void Space::Scope::rmNull(std::string name) {
    values[pos(name)].second.removeNull();
}
void Space::Scope::changeList(std::string name) {
    values[pos(name)].second.islist = !values[pos(name)].second.islist;
}

// Object
Space::Scope::Object::Object(std::string name, objKind k, int ln, int col): objName(name), kind(k), line(ln), column(col) {}
Space::Scope::Object::Object(std::string name, objKind k, std::vector<ygen::byteCode> c, std::vector<std::string> cp, int ln, int col):
                            objName(name), kind(k), codes(c), constpool(cp), line(ln), column(col) {}
Space::Scope::Object::Object(std::string name, std::vector<std::pair<ygen::paraHelper::type, std::string>> p, int ln, int col):
                            objName(name), paras(p), kind(objKind::funcObj), line(ln), column(col) {}
Space::Scope::Object::Object(std::string name, std::vector<ygen::byteCode> c, std::vector<std::string> cp, std::vector<std::pair<ygen::paraHelper::type, std::string>> p, int ln, int col):
                            objName(name), paras(p), codes(c), constpool(cp), kind(objKind::funcObj), line(ln), column(col) {}

Space::Scope::Value Space::Scope::Object::getMember(std::string name) {
    if(!isTypableObj())
        throw yoexception::YoError("TypeError", "You cannot manipulate this Object as a class", line, column);
    if(objSpace->find(name))
        return objSpace->getValue(name);
    else
        throw yoexception::YoError("NameError", "Cannot find identifier named: '" + name + "'", line, column);
}

yvm::YVM Space::Scope::Object::call() {
    if(!isFuncObj())
        throw yoexception::YoError("TypeError", "You cannot call an Object of a non-function type as a function", line, column);
    objVM->reload(codes, constpool);
    objVM->run("null");
    return *objVM;
}

bool Space::Scope::Object::isFuncObj() {
    return kind == objKind::funcObj? true:false;
}
bool Space::Scope::Object::isTypableObj() {
    return kind == objKind::typableObj? true:false;
}
// Value
Space::Scope::Value::Value(bool isc, int ln, int col):
        isnull(true), isconst(isc), type(ygen::paraHelper::null), line(ln), column(col) {}
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
Space::Scope::Value::Value(Object val, bool isc, int ln, int col):
                objvalue(val), isconst(isc), type(ygen::paraHelper::boolean), line(ln), column(col), isobj(true) {}
Space::Scope::Value::Value(Value* _ref, bool isc, int ln, int col): 
                isconst(isc), isref(true), type(ygen::paraHelper::ref), line(ln), column(col) {
                    ref = new Value;
                    ref = _ref;
                }

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
bool Space::Scope::Value::isRef(){
    return isref;
}
bool Space::Scope::Value::isObj(){
    return isobj;
}
bool Space::Scope::Value::isNull(){
    return isnull;
}
ygen::paraHelper::type Space::Scope::Value::getType(){
    return type;
}
void Space::Scope::Value::removeNull() {
    isnull = false;
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
Space::Scope::Object Space::Scope::Value::getObjectValue() {
    return objvalue;
}
Space::Scope::Value* Space::Scope::Value::getRef() {
    return ref;
}
Space::Scope::Value* Space::Scope::Value::getSelfRef() {
    auto self = new Value;
    self = this;
    return self;
}
std::vector<Space::Scope::Value> Space::Scope::Value::getList(){
    return list;
}

void Space::Scope::Value::assignInt(int value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    intvalue = value;
}
void Space::Scope::Value::assignDeci(float value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    decivalue = value;
}
void Space::Scope::Value::assignBool(bool value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    boolvalue = value;
}
void Space::Scope::Value::assignString(std::string value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    strvalue = value;
}
void Space::Scope::Value::assignObject(Object value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    objvalue = value;
}
void Space::Scope::Value::assignChar(char value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    charvalue = value;
}
void Space::Scope::Value::refAnother(Value value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    ref = &value;
}
void Space::Scope::Value::assignValue(Value value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    this->boolvalue = value.boolvalue;
    this->charvalue = value.charvalue;
    this->intvalue = value.intvalue;
    this->decivalue = value.decivalue;
    this->strvalue = value.strvalue;
    this->isconst = value.isconst;
    this->islist = value.islist;
    this->line = value.line;
    this->column = value.column;
}

void Space::Scope::Value::assignListValue(std::vector<Value> value) {
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
int yvm::YVM::addValueList(std::vector<Space::Scope::Value> list){
    std::vector<vmValue> newlist;
    if((vmVType)list[0].getType() == vmVType::integer){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue((vmVType)list[i].getType(), list[i].getIntValue()));
        }
    }
    else if((vmVType)list[0].getType() == vmVType::decimal){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue((vmVType)list[i].getType(), list[i].getDeciValue()));
        }
    }
    else if((vmVType)list[0].getType() == vmVType::boolean){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue((vmVType)list[i].getType(), list[i].getBoolValue()));
        }
    }
    else if((vmVType)list[0].getType() == vmVType::string){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue((vmVType)list[i].getType(), addString(list[i].getStrValue())));
        }
    }
    else if((vmVType)list[0].getType() == vmVType::character){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue((vmVType)list[i].getType(), addChar(list[i].getCharValue())));
        }
    }
    listpool.push_back(newlist);
    return listpool.size() - 1;
}
int yvm::YVM::addChar(char ch){
    std::string str;
    str.push_back(ch);
    constpool.push_back(str);
    return constpool.size()-1;
}
std::vector<std::string> yvm::YVM::split(std::string str, char sp) {
    // 此处实现照抄console::tools
    std::vector<std::string> ret;
    int i = 0;
    while(i < str.size()) {
        std::string content;
        for(; i < str.size(); i ++) {
            if(str[i] == sp) {i++; break;}
            content.push_back(str[i]);
        }
        ret.push_back(content);
    }
    return ret;
}

std::vector<std::string> yvm::YVM::getConstPool(){
    return constpool;
}
std::vector<std::vector<yvm::YVM::vmValue>> yvm::YVM::getListPool(){
    return listpool;
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

// BIF
yvm::YVM::vmValue yvm::YVM::bifPrint(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    auto content = paras[0];
    if(content.first == vmVType::string || content.first == vmVType::character)
        std::cout<<constpool[content.second];
    else if(content.first == vmVType::list){
        auto list = listpool[content.second];
        for(int j = 0; j < list.size(); j++){
            auto elt = list[j];
            if(elt.first == vmVType::string || elt.first == vmVType::character)
                std::cout<<constpool[elt.second];
            else
                std::cout<<elt.second;
        }
    }
    else
        std::cout<<content.second;
    return vmValue(vmVType::null, 0.0);
}
yvm::YVM::vmValue yvm::YVM::bifPrintLn(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    auto content = paras[0];
    if(content.first == vmVType::string || content.first == vmVType::character)
        std::cout<<constpool[content.second];
    else if(content.first == vmVType::list){
        auto list = listpool[content.second];
        for(int j = 0; j < list.size(); j++){
            auto elt = list[j];
            if(elt.first == vmVType::string || elt.first == vmVType::character)
                std::cout<<constpool[elt.second];
            else
                std::cout<<elt.second;
        }
    }
    else
        std::cout<<content.second;
    std::cout<<std::endl;
    return vmValue(vmVType::null, 0.0);
}
yvm::YVM::vmValue yvm::YVM::bifLen(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    if(paras[0].first != vmVType::list && paras[0].first != vmVType::string)
        throw yoexception::YoError("TypeError", "This function does not support arguments of this type", line, column);
    vmValue ret;
    switch (paras[0].first) {
        case vmVType::list: ret = vmValue(vmVType::integer, listpool[paras[0].second].size()); break;
        case vmVType::string: ret = vmValue(vmVType::integer, constpool[paras[0].second].size()); break;
        default: break;
    }
    return ret;
}
yvm::YVM::vmValue yvm::YVM::bifSys(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    if(paras[0].first != vmVType::string)
        throw yoexception::YoError("TypeError", "This function does not support arguments of this type", line, column);
    system(constpool[paras[0].second].c_str());
    return vmValue(vmVType::null, 0.0);
}
yvm::YVM::vmValue yvm::YVM::bifInput(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty() || paras.size() == 1) {
        if(paras.size() == 1)
            std::cout<<constpool[paras[0].second];
        std::string text;
        std::getline(std::cin, text);
        return vmValue(vmVType::string, addString(text));
    }
    else throw yoexception::YoError("ParaError", "Too many parameters", line, column);
}
yvm::YVM::vmValue yvm::YVM::bifToInteger(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    vmValue value;
    switch (paras[0].first) {
        case vmVType::integer: value = vmValue(vmVType::integer, paras[0].second); break;
        case vmVType::decimal: value = vmValue(vmVType::integer, (int)paras[0].second); break;
        case vmVType::boolean: value = vmValue(vmVType::integer, (int)paras[0].second); break;
        case vmVType::string: value = vmValue(vmVType::integer, std::atoi(constpool[paras[0].second].c_str())); break;
        case vmVType::character: value = vmValue(vmVType::integer, (int)constpool[paras[0].second][0]); break;
        default: value = vmValue(vmVType::integer, 0); break;
    }
    return value;
}
yvm::YVM::vmValue yvm::YVM::bifToString(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    vmValue value;
    switch (paras[0].first) {
        case vmVType::integer: {
            std::stringstream oss;
            oss << paras[0].second;
            value = vmValue(vmVType::string, addString(oss.str()));
            break;
        }
        case vmVType::decimal: {
            std::stringstream oss;
            oss << paras[0].second;
            value = vmValue(vmVType::string, addString(oss.str()));
            break;
        }
        case vmVType::boolean: value = vmValue(vmVType::string, addString((bool)paras[0].second?"true":"false")); break;
        case vmVType::string: value = vmValue(vmVType::string, paras[0].second); break;
        case vmVType::character: value = vmValue(vmVType::string, paras[0].second); break;
        default: value = vmValue(vmVType::string, addString("null")); break;
    }
    return value;
}
yvm::YVM::vmValue yvm::YVM::bifToDecimal(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    vmValue value;
    switch (paras[0].first) {
        case vmVType::integer: value = vmValue(vmVType::decimal, (float)paras[0].second); break;
        case vmVType::decimal: value = vmValue(vmVType::decimal, paras[0].second); break;
        case vmVType::boolean: value = vmValue(vmVType::decimal, (float)paras[0].second); break;
        case vmVType::string: value = vmValue(vmVType::decimal, (float)atof(constpool[paras[0].second].c_str())); break;
        case vmVType::character: value = vmValue(vmVType::decimal, (float)atof(constpool[paras[0].second].c_str())); break;
        default: value = vmValue(vmVType::decimal, 0.0); break;
    }
    return value;
}
yvm::YVM::vmValue yvm::YVM::bifToBoolean(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    vmValue value;
    switch (paras[0].first) {
        case vmVType::integer: value = vmValue(vmVType::boolean, (bool)paras[0].second); break;
        case vmVType::decimal: value = vmValue(vmVType::boolean, (bool)paras[0].second); break;
        case vmVType::boolean: value = vmValue(vmVType::boolean, (bool)paras[0].second); break;
        case vmVType::string: value = vmValue(vmVType::boolean, constpool[paras[0].second] == "true"? true: false); break;
        case vmVType::character: throw yoexception::YoError("TypeError", "cannot convert type 'Character' to 'Boolean'", line, column); break;
        default: value = vmValue(vmVType::boolean, false); break;
    }
    return value;
}
//

int yvm::YVM::run(std::string arg) {
    for(int i = 0; i < codes.size(); i++) {
        switch (codes[i].code)
        {
            case ygen::btc::push:{
                if(codes[i].arg2 == vmVType::iden){
                    if(codes[i + 1].code == ygen::btc::idenend){
                        // 单个identifier，直接push
                        auto name = constpool[codes[i].arg1];
                        if(runtimeSpace.find(name)){
                            auto type = (vmVType)runtimeSpace.getValue(name).getType();
                            if(!runtimeSpace.getValue(name).isList()){
                                switch (type)
                                {
                                    case vmVType::null: envPush(vmValue(type, 0));
                                    case vmVType::integer: envPush(vmValue(type, runtimeSpace.getValue(name).getIntValue())); break;
                                    case vmVType::decimal: envPush(vmValue(type, runtimeSpace.getValue(name).getDeciValue())); break;
                                    case vmVType::boolean: envPush(vmValue(type, runtimeSpace.getValue(name).getBoolValue())); break;
                                    case vmVType::string: envPush(vmValue(type, addString(runtimeSpace.getValue(name).getStrValue()))); break;
                                    case vmVType::character: envPush(vmValue(type, addChar(runtimeSpace.getValue(name).getCharValue()))); break;
                                    case vmVType::ref: {
                                        // 嵌套处理
                                        auto value = runtimeSpace.getValue(name).getRef();
                                        auto refType = (vmVType)value->getType();
                                        if(!value->isList()){
                                            switch (refType)
                                            {
                                                case vmVType::integer: envPush(vmValue(refType, value->getIntValue())); break;
                                                case vmVType::decimal: envPush(vmValue(refType, value->getDeciValue())); break;
                                                case vmVType::boolean: envPush(vmValue(refType, value->getBoolValue())); break;
                                                case vmVType::string: envPush(vmValue(refType, addString(value->getStrValue()))); break;
                                                case vmVType::character: envPush(vmValue(refType, addChar(value->getCharValue()))); break;
                                                default: throw yoexception::YoError("TypeError","Unsupported type used",codes[i].line, codes[i].column); break;
                                            }
                                        }
                                        else{
                                            envPush(vmValue(vmVType::list, addValueList(value->getList())));
                                        }
                                        break;
                                    }
                                    default: throw yoexception::YoError("TypeError","Unsupported type used",codes[i].line, codes[i].column); break;
                                }
                            }
                            else{
                                envPush(vmValue(vmVType::list, addValueList(runtimeSpace.getValue(name).getList())));
                            }
                        }
                        else throw yoexception::YoError("NameError", "Cannot find identifier named: '" + constpool[codes[i].arg1] + "'", codes[i].line, codes[i].column);
                    }
                    else ; // 多个identifier连接在一起
                }
                else if(codes[i].arg2 == vmVType::ref)
                    envPush(vmValue(vmVType::ref, codes[i].arg1));
                else envPush(vmValue((vmVType)codes[i].arg2, codes[i].arg1));
                break;
            }
            case ygen::btc::call: {
                std::vector<vmValue> paras;
                bool hasPara = false;
                auto name = constpool[envPop().second]; // funcName
                if(envPeek().first == vmVType::flag && constpool[envPeek().second] == "PARAENDFLAG")
                    hasPara = true;
                envPop(); // 删除flag
                if(hasPara) {
                    // 有参数，开始制作参数列表
                    while(envPeek().first != vmVType::flag && !runtimeStack.empty()) {
                        paras.push_back(envPop());
                    }
                    envPop(); // 删除参数末尾的flag
                }
                std::reverse(paras.begin(), paras.end()); // 出来的参数顺序颠倒
                if(std::find(bifNames.begin(), bifNames.end(), name) != bifNames.end()) {
                    // 找到了bifname，是bif，交给bif处理
                    if(name == "print")
                        envPush(bifPrint(paras, codes[i].line, codes[i].column));
                    else if(name == "println")
                        envPush(bifPrintLn(paras, codes[i].line, codes[i].column));
                    else if(name == "len")
                        envPush(bifLen(paras, codes[i].line, codes[i].column));
                    else if(name == "sys")
                        envPush(bifSys(paras, codes[i].line, codes[i].column));
                    else if(name == "input")
                        envPush(bifInput(paras, codes[i].line, codes[i].column));
                    else if(name == "toInt")
                        envPush(bifToInteger(paras, codes[i].line, codes[i].column));
                    else if(name == "toStr")
                        envPush(bifToString(paras, codes[i].line, codes[i].column));
                    else if(name == "toDeci")
                        envPush(bifToDecimal(paras, codes[i].line, codes[i].column));
                    else if(name == "toBool")
                        envPush(bifToBoolean(paras, codes[i].line, codes[i].column));
                }
                break;
            }
            case ygen::btc::listend:{
                envPush(vmValue(vmVType::flag, addString("LISTENDFLAG")));
                break;
            }
            case ygen::btc::paraend:{
                envPush(vmValue(vmVType::flag, addString("PARAENDFLAG")));
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
                    if(index.second > listnew.size() - 1)
                        throw yoexception::YoError("ListError", "Index out of range of list", codes[i].line, codes[i].column);
                    envPush(vmValue(listnew[0].first, listnew[index.second].second));
                }   
                else throw yoexception::YoError("SyntaxError", "Cannot index an object that is not a composite object", codes[i].line, codes[i].column);
                break;
            }
            case ygen::btc::selfadd:{
                auto iden = envPop();
                bool isFront = codes[i].arg1 == 1.0 ? true : false;
                if(runtimeSpace.getValue(constpool[iden.second]).getType() != ygen::paraHelper::integer) 
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column);
                
                if(isFront) {
                    auto temp = runtimeSpace.getValue(constpool[iden.second]).getIntValue() + 1;
                    runtimeSpace.assignValue(constpool[iden.second], 
                        temp);
                    envPush(vmValue(vmVType::integer, runtimeSpace.getValue(constpool[iden.second]).getIntValue()));
                }
                else {
                    auto temp = runtimeSpace.getValue(constpool[iden.second]).getIntValue() + 1;
                    envPush(vmValue(vmVType::integer, runtimeSpace.getValue(constpool[iden.second]).getIntValue()));
                    runtimeSpace.assignValue(constpool[iden.second], 
                        temp);
                }

                break;
            }
            case ygen::btc::selfsub:{
                auto iden = envPop();
                bool isFront = codes[i].arg1 == 1.0 ? true : false;
                if(runtimeSpace.getValue(constpool[iden.second]).getType() != ygen::paraHelper::integer) 
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column);
                
                if(isFront) {
                    auto temp = runtimeSpace.getValue(constpool[iden.second]).getIntValue() - 1;
                    runtimeSpace.assignValue(constpool[iden.second], 
                        temp);
                    envPush(vmValue(vmVType::integer, runtimeSpace.getValue(constpool[iden.second]).getIntValue()));
                }
                else {
                    auto temp = runtimeSpace.getValue(constpool[iden.second]).getIntValue() - 1;
                    envPush(vmValue(vmVType::integer, runtimeSpace.getValue(constpool[iden.second]).getIntValue()));
                    runtimeSpace.assignValue(constpool[iden.second], 
                        temp);
                }

                break;
            }
            case ygen::btc::add:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::integer, left.second + right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::decimal, left.second + right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::integer, left.second + right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::decimal, left.second + right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::string: {
                        switch (right.first) {
                            case vmVType::string: envPush(vmValue(vmVType::string, addString(constpool[left.second] + constpool[right.second]))); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::character: {
                        switch (right.first) {
                            case vmVType::string: envPush(vmValue(vmVType::string, addString(constpool[left.second] + constpool[right.second]))); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::sub:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::integer, left.second - right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::decimal, left.second - right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::integer, left.second - right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::decimal, left.second - right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::div:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::integer, left.second - right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::decimal, left.second - right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::integer, left.second - right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::decimal, left.second - right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::mul:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::integer, left.second * right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::decimal, left.second * right.second)); break;
                            case vmVType::string: {
                                auto str = constpool[right.second];
                                for(int i = 1; i < left.second; i ++) {
                                    str += str;
                                }
                                envPush(vmValue(vmVType::string, addString(str)));
                                break;
                            }
                            case vmVType::character: {
                                auto str = constpool[right.second];
                                for(int i = 1; i < left.second; i ++) {
                                    str += str;
                                }
                                envPush(vmValue(vmVType::string, addString(str)));
                                break;
                            }
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::integer, left.second * right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::decimal, left.second * right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::string: {
                        switch (right.first) {
                            case vmVType::integer: {
                                auto str = constpool[left.second];
                                for(int i = 1; i < right.second; i ++) {
                                    str += str;
                                }
                                envPush(vmValue(vmVType::string, addString(str)));
                                break;
                            }
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::character: {
                        switch (right.first) {
                            case vmVType::integer: {
                                auto str = constpool[left.second];
                                for(int i = 1; i < right.second; i ++) {
                                    str += str;
                                }
                                envPush(vmValue(vmVType::string, addString(str)));
                                break;
                            }
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    default: break;
                }
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
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second == right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second == right.second)); break;
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 0.0)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second == right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second == right.second)); break;
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 0.0)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::string: {
                        switch (right.first) {
                            case vmVType::string: envPush(vmValue(vmVType::boolean, constpool[left.second] == constpool[right.second])); break;
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 0.0)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::character: {
                        switch (right.first) {
                            case vmVType::string: envPush(vmValue(vmVType::boolean, constpool[left.second] == constpool[right.second])); break;
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 0.0)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::boolean: {
                        switch (right.first) {
                            case vmVType::boolean: envPush(vmValue(vmVType::boolean, constpool[left.second] == constpool[right.second])); break;
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 0.0)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::null: {
                        switch (right.first) {
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 1.0)); break;
                            default: envPush(vmValue(vmVType::boolean, 0.0)); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::noequ:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 1.0)); break;
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second != right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second != right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 1.0)); break;
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second != right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second != right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::string: {
                        switch (right.first) {
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 1.0)); break;
                            case vmVType::string: envPush(vmValue(vmVType::boolean, constpool[left.second] != constpool[right.second])); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::character: {
                        switch (right.first) {
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 1.0)); break;
                            case vmVType::string: envPush(vmValue(vmVType::boolean, constpool[left.second] != constpool[right.second])); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::boolean: {
                        switch (right.first) {
                            case vmVType::boolean: envPush(vmValue(vmVType::boolean, constpool[left.second] != constpool[right.second])); break;
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 0.0)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::null: {
                        switch (right.first) {
                            case vmVType::null: envPush(vmValue(vmVType::boolean, 0.0)); break;
                            default: envPush(vmValue(vmVType::boolean, 1.0)); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::gt:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second > right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second > right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second > right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second > right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::gtet:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second >= right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second >= right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second >= right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second >= right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::lt:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second < right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second < right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second < right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second < right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::ltet:{
                auto right = envPop();
                auto left = envPop();
                switch (left.first) {
                    case vmVType::integer: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second <= right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second <= right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    case vmVType::decimal: {
                        switch (right.first) {
                            case vmVType::integer: envPush(vmValue(vmVType::boolean, left.second <= right.second)); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::boolean, left.second <= right.second)); break;
                            default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column); break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::no:{
                envPush(vmValue(vmVType::boolean, envPop().second == true?0:1));
                break;
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
            case ygen::btc::jmp:{
                switch((ygen::paraHelper::jmpt)codes[i].arg1) {
                    case ygen::paraHelper::jmpt::reqTrue: {
                        switch((ygen::paraHelper::jmpt)codes[i].arg2) {
                            case ygen::paraHelper::jmpt::outWscope: {
                                auto cond = envPop();
                                if((bool)cond.second == true); // 条件为真进入循环体
                                else {
                                    int state = 0;
                                    while(true) {
                                        i ++;
                                        if(codes[i].code == ygen::scopestart) state ++;
                                        else if(codes[i].code == ygen::scopeend) state --;

                                        if(state == 0) break;
                                    }
                                    
                                }
                                break;
                            }
                            case ygen::paraHelper::jmpt::outScopeDirectly: {
                                auto cond = envPop();
                                if((bool)cond.second == true); // 条件为真进入循环体
                                else {
                                    int state = 1;
                                    while(true) {
                                        i ++;
                                        if(codes[i].code == ygen::scopestart) state ++;
                                        else if(codes[i].code == ygen::scopeend) state --;

                                        if(state == 0) break;
                                    }

                                }
                                break;
                            }
                            case ygen::paraHelper::jmpt::outIFscope: {
                                auto cond = envPop();
                                if((bool)cond.second == true) envPush(vmValue(vmVType::boolean, 1.0)); // 条件为真进入
                                else {
                                    int state = 0;
                                    while(true) {
                                        i ++;
                                        if(codes[i].code == ygen::scopestart) state ++;
                                        else if(codes[i].code == ygen::scopeend) state --;

                                        if(state == 0) break;
                                    }
                                    envPush(vmValue(vmVType::boolean, 0));
                                }
                                break;
                            }
                            case ygen::paraHelper::jmpt::outElifscope: {
                                auto cond = envPop();
                                if(runtimeStack.empty()) 
                                    throw yoexception::YoError("Syntax", "You cannot use elif statements directly without an if statement", codes[i].line, codes[i].column);
                                else if(envPeek().first != vmVType::boolean)
                                    throw yoexception::YoError("Syntax", "You cannot use elif statements directly without an if statement", codes[i].line, codes[i].column);
                                if((bool)cond.second == true && (bool)envPop().second == false) envPush(vmValue(vmVType::boolean, 1.0)); // 条件为真进入
                                else {
                                    int state = 0;
                                    while(true) {
                                        i ++;
                                        if(codes[i].code == ygen::scopestart) state ++;
                                        else if(codes[i].code == ygen::scopeend) state --;

                                        if(state == 0) break;
                                    }
                                    envPush(vmValue(vmVType::boolean, 0));
                                }
                                break;
                            }
                            case ygen::paraHelper::jmpt::outElsescope: {
                                if(runtimeStack.empty()) 
                                    throw yoexception::YoError("Syntax", "You cannot use else statements directly without an if statement", codes[i].line, codes[i].column);
                                else if(envPeek().first != vmVType::boolean)
                                    throw yoexception::YoError("Syntax", "You cannot use else statements directly without an if statement", codes[i].line, codes[i].column);
                                if((bool)envPop().second == false) envPush(vmValue(vmVType::boolean, 1.0)); // 条件为真进入
                                else {
                                    int state = 0;
                                    while(true) {
                                        i ++;
                                        if(codes[i].code == ygen::scopestart) state ++;
                                        else if(codes[i].code == ygen::scopeend) state --;

                                        if(state == 0) break;
                                    }
                                    envPush(vmValue(vmVType::boolean, 0));
                                }
                                break;
                            }
                            case ygen::paraHelper::jmpt::findSStart: {
                                auto cond = envPop();
                                if((bool)cond.second == true) {
                                    int state = 0;
                                    while(true) {
                                        i --;
                                        auto temp = codes[i].code;
                                        if(codes[i].code == ygen::scopestart) state --;
                                        else if(codes[i].code == ygen::scopeend) state ++;

                                        if(state == 0) break;
                                    }
                                    i--;
                                }
                                else ;
                                break;
                            }
                            default: break;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case ygen::btc::stf:{
                if(runtimeStack.empty());
                else{
                    auto para = envPop();
                    auto name = constpool[codes[i].arg1];
                    if(name == "typeof") {
                        switch (para.first)
                        {
                            case vmVType::integer: envPush(vmValue(vmVType::string, addString("<integer>"))); break;
                            case vmVType::decimal: envPush(vmValue(vmVType::string, addString("<decimal>"))); break;
                            case vmVType::boolean: envPush(vmValue(vmVType::string, addString("<boolean>"))); break;
                            case vmVType::string: envPush(vmValue(vmVType::string, addString("<string>"))); break;
                            case vmVType::character: envPush(vmValue(vmVType::string, addString("<char>"))); break;
                            case vmVType::null: envPush(vmValue(vmVType::string, addString("<null>"))); break;
                            default: break;
                        }
                    }
                    else if(name == "nameof") {
                        if(para.first != vmVType::string)
                            throw yoexception::YoError("TypeError", "Only values of type '<string>' are accepted within this operator", codes[i].line, codes[i].column);
                        auto iname = constpool[para.second];
                        auto value = runtimeSpace.getValue(iname);
                        if(value.isList()){
                            auto list = value.getList();
                            std::vector<vmValue> retlist;
                            for(int i = 0; i < list.size(); i ++) {
                                switch ((vmVType)list[i].getType())
                                {
                                    case vmVType::integer: retlist.push_back(vmValue(vmVType::integer, list[i].getIntValue())); break;
                                    case vmVType::decimal: retlist.push_back(vmValue(vmVType::decimal, list[i].getDeciValue())); break;
                                    case vmVType::boolean: retlist.push_back(vmValue(vmVType::boolean, list[i].getBoolValue())); break;
                                    case vmVType::string: retlist.push_back(vmValue(vmVType::string, addString(list[i].getStrValue()))); break;
                                    case vmVType::character: retlist.push_back(vmValue(vmVType::character, addChar(list[i].getCharValue()))); break;
                                    default: break;
                                }
                            }
                            envPush(vmValue(vmVType::list, addList(retlist)));
                        }
                        else{
                            switch ((vmVType)value.getType())
                            {
                                case vmVType::integer: envPush(vmValue(vmVType::integer, value.getIntValue())); break;
                                case vmVType::decimal: envPush(vmValue(vmVType::decimal, value.getDeciValue())); break;
                                case vmVType::boolean: envPush(vmValue(vmVType::boolean, value.getBoolValue())); break;
                                case vmVType::string: envPush(vmValue(vmVType::string, addString(value.getStrValue()))); break;
                                case vmVType::character: envPush(vmValue(vmVType::character, addChar(value.getCharValue()))); break;
                                default: break;
                            }
                        }
                    }
                }
                break;
            }
            case ygen::btc::out:{
                auto content = envPop();
                if(content.first == vmVType::string || content.first == vmVType::character)
                    std::cout<<constpool[content.second];
                else if(content.first == vmVType::list){
                    auto list = listpool[content.second];
                    for(int j = 0; j < list.size(); j++){
                        auto elt = list[j];
                        if(elt.first == vmVType::string || elt.first == vmVType::character)
                            std::cout<<constpool[elt.second];
                        else
                            std::cout<<elt.second;
                    }
                }
                else
                    std::cout<<content.second;
                break;
            }
            case ygen::btc::define:{
                if(runtimeSpace.find(constpool[codes[i].arg1])) // 检测是否有重复的标识符
                    throw yoexception::YoError("NameError", "Duplicate identifier: '" + constpool[codes[i].arg1] + "'", codes[i].line, codes[i].column);
                auto &current = runtimeSpace.current();
                current.memberlist.push_back(constpool[codes[i].arg1]);
                break;
            }
            case ygen::btc::assign:{
                auto value = envPop();
                auto name = envPop();
                if(!runtimeSpace.find(constpool[name.second]))
                    throw yoexception::YoError("NameError", "There is no identifier named: '" + constpool[name.second] + "'", codes[i].line, codes[i].column);
                if(value.first != (vmVType)runtimeSpace.getValue(constpool[name.second]).getType() &&
                        !runtimeSpace.getValue(constpool[name.second]).isNull())
                    throw yoexception::YoError("TypeError", "The type before and after assignment is inconsistent!", codes[i].line, codes[i].column);
                if(codes[i].arg1 == 1.0) {
                    // 是数组元素赋值
                    auto index = envPop();
                    if(!runtimeSpace.getValue(constpool[name.second]).isList())
                        throw yoexception::YoError("TypeError", "Object does not support the operation of assigning values to members", codes[i].line, codes[i].column);
                    auto list = runtimeSpace.getValue(constpool[name.second]).getList();
                    if(index.second > list.size() - 1)
                        throw yoexception::YoError("ListError", "Index out of range of list", codes[i].line, codes[i].column);
                    switch (value.first)
                    {
                        case vmVType::integer: list[index.second] = Space::Scope::Value((int)value.second, true, codes[i].line, codes[i].column); break;
                        case vmVType::decimal: list[index.second] = Space::Scope::Value((float)value.second, true, codes[i].line, codes[i].column); break;
                        case vmVType::boolean: list[index.second] = Space::Scope::Value((bool)value.second, true, codes[i].line, codes[i].column); break;
                        case vmVType::string: list[index.second] = Space::Scope::Value(constpool[value.second], true, codes[i].line, codes[i].column); break;
                        case vmVType::character: list[index.second] = Space::Scope::Value(constpool[value.second][0], true, codes[i].line, codes[i].column); break;
                        default: break;
                    }
                    runtimeSpace.assignValue(constpool[name.second], list);
                }
                else {
                    // 普通赋值
                    switch (value.first)
                    {
                        case vmVType::integer:
                            if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                runtimeSpace.rmValueNull(constpool[name.second]);
                                runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::integer);
                            }
                            runtimeSpace.assignValue(constpool[name.second], (int)value.second);
                            break;
                        case vmVType::decimal:
                            if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                runtimeSpace.rmValueNull(constpool[name.second]);
                                runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::decimal);
                            }
                            runtimeSpace.assignValue(constpool[name.second], (float)value.second);
                            break;
                        case vmVType::boolean:
                            if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                runtimeSpace.rmValueNull(constpool[name.second]);
                                runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::boolean);
                            }
                            runtimeSpace.assignValue(constpool[name.second], (bool)value.second);
                            break;
                        case vmVType::string:
                            if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                runtimeSpace.rmValueNull(constpool[name.second]);
                                runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::string);
                            }
                            runtimeSpace.assignValue(constpool[name.second], constpool[value.second]);
                            break;
                        case vmVType::character:
                            if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                runtimeSpace.rmValueNull(constpool[name.second]);
                                runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::character);
                            }
                            runtimeSpace.assignValue(constpool[name.second], constpool[value.second][0]);
                            break;
                        case vmVType::list: {
                            if(runtimeSpace.getValue(constpool[name.second]).isNull())
                                runtimeSpace.toggleList(constpool[name.second]);
                            auto list = listpool[value.second];
                            if(list.size() > runtimeSpace.getValue(constpool[name.second]).getList().size() && !runtimeSpace.getValue(constpool[name.second]).isNull())
                                throw yoexception::YoError("AssignError", "An array cannot be resized at will", codes[i].line, codes[i].column);
                            std::vector<Space::Scope::Value> retlist;
                            for(auto elt: list) {
                                switch (elt.first)
                                {
                                    case vmVType::integer:
                                        if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                            runtimeSpace.rmValueNull(constpool[name.second]);
                                            runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::integer);
                                        }
                                        retlist.push_back(Space::Scope::Value((int)elt.second, false, codes[i].line, codes[i].column));
                                        break;
                                    case vmVType::decimal:
                                        if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                            runtimeSpace.rmValueNull(constpool[name.second]);
                                            runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::decimal);
                                        }
                                        retlist.push_back(Space::Scope::Value((float)elt.second, false, codes[i].line, codes[i].column));
                                        break;
                                    case vmVType::boolean:
                                        if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                            runtimeSpace.rmValueNull(constpool[name.second]);
                                            runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::boolean);
                                        }
                                        retlist.push_back(Space::Scope::Value((bool)elt.second, false, codes[i].line, codes[i].column));
                                        break;
                                    case vmVType::string:
                                        if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                            runtimeSpace.rmValueNull(constpool[name.second]);
                                            runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::string);
                                        }
                                        retlist.push_back(Space::Scope::Value(constpool[elt.second], false, codes[i].line, codes[i].column));
                                        break;
                                    case vmVType::character:
                                        if(runtimeSpace.getValue(constpool[name.second]).isNull()) {
                                            runtimeSpace.rmValueNull(constpool[name.second]);
                                            runtimeSpace.reWriteType(constpool[name.second], (ygen::paraHelper::type)vmVType::character);
                                        }
                                        retlist.push_back(Space::Scope::Value(constpool[elt.second][0], false, codes[i].line, codes[i].column));
                                        break;
                                    default: break;
                                }
                            }
                            runtimeSpace.assignValue(constpool[name.second], retlist); break;
                        }
                    default: break;
                    }
                }
                envPush(vmValue(vmVType::null, 0.0));
                break;
            }     
            case ygen::btc::init:{
                auto name = constpool[codes[i].arg1];
                if(std::count(runtimeSpace.current().memberlist.begin(), runtimeSpace.current().memberlist.end(), name)){
                    auto value = envPop();
                    //type-checker
                    if(codes[i].arg4 == 1.0){
                        auto reqType = constpool[codes[i].arg3];
                        if(value.first == vmVType::null){
                            // null 如果给了类型指定，存储在量中
                            if(reqType == "integer")
                                runtimeSpace.createValue(name, Space::Scope::Value(0, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            else if(reqType == "decimal")
                                runtimeSpace.createValue(name, Space::Scope::Value((float)0.0f, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            else if(reqType == "boolean")
                                runtimeSpace.createValue(name, Space::Scope::Value(false, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            else if(reqType == "string")
                                runtimeSpace.createValue(name, Space::Scope::Value((std::string)"\0", constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            else if(reqType == "char")
                                runtimeSpace.createValue(name, Space::Scope::Value('\0', constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            break;
                        }
                        else{
                            if(reqType == "integer" && value.first == vmVType::integer);
                            else if(reqType == "decimal" && value.first == vmVType::decimal);
                            else if(reqType == "string" && value.first == vmVType::string);
                            else if(reqType == "char" && value.first == vmVType::character);
                            else if(reqType == "boolean" && value.first == vmVType::boolean);
                            else {
                                auto v = listpool[value.second];
                                if(reqType == "integer[]" && v[0].first == vmVType::integer);
                                else if(reqType == "decima[]l" && v[0].first == vmVType::decimal);
                                else if(reqType == "string[]" && v[0].first == vmVType::string);
                                else if(reqType == "char[]" && v[0].first == vmVType::character);
                                else if(reqType == "boolean[]" && v[0].first == vmVType::boolean);
                                else{
                                    error_type:
                                    // 不允许创建该变量，于是删除对应memberlist中的元素
                                    auto &scope = runtimeSpace.current();
                                    for(int i = 0; i < scope.memberlist.size(); i++) {
                                        if(scope.memberlist[i] == name)
                                            scope.memberlist.erase(scope.memberlist.begin() + i);
                                    }
                                    throw yoexception::YoError("TypeError", "The expected type does not match the type given by the actual expression", codes[i].line, codes[i].column);
                                }
                            }
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
                    else if(value.first == vmVType::null)
                        runtimeSpace.createValue(name, Space::Scope::Value(constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                    else if(value.first == vmVType::ref){
                        auto refName = constpool[value.second];
                        runtimeSpace.createValue(name, Space::Scope::Value(runtimeSpace.getValueSelfRef(refName), constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                    }
                    else if(value.first == vmVType::list) {
                        auto list = listpool[value.second];
                        std::vector<Space::Scope::Value> newlist;
                        if(list.empty())
                            throw yoexception::YoError("InitError", "Initializing an empty list is currently not allowed", codes[i].line, codes[i].column);
                        if(list[0].first == vmVType::integer){
                            for(int j = 0; j < list.size(); j++){
                                if(list[j].first != list[j - 1].first && j > 0) // 当前后类型不一致时，报错
                                    throw yoexception::YoError("TypeError", "All elements in the list must be of the same type", codes[i].line, codes[i].column);
                                newlist.push_back(Space::Scope::Value((int)list[j].second, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            }
                        }
                        else if(list[0].first == vmVType::boolean){
                            for(int j = 0; j < list.size(); j++){
                                if(list[j].first != list[j - 1].first && j > 0) // 当前后类型不一致时，报错
                                    throw yoexception::YoError("TypeError", "All elements in the list must be of the same type", codes[i].line, codes[i].column);
                                newlist.push_back(Space::Scope::Value((bool)list[j].second, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            }
                        }
                        else if(list[0].first == vmVType::decimal){
                            for(int j = 0; j < list.size(); j++){
                                if(list[j].first != list[j - 1].first && j > 0) // 当前后类型不一致时，报错
                                    throw yoexception::YoError("TypeError", "All elements in the list must be of the same type", codes[i].line, codes[i].column);
                                newlist.push_back(Space::Scope::Value((float)list[j].second, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            }
                        }
                        else if(list[0].first == vmVType::string){
                            for(int j = 0; j < list.size(); j++){
                                if(list[j].first != list[j - 1].first && j > 0) // 当前后类型不一致时，报错
                                    throw yoexception::YoError("TypeError", "All elements in the list must be of the same type", codes[i].line, codes[i].column);
                                newlist.push_back(Space::Scope::Value(constpool[list[j].second], constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            }
                        }
                        else if(list[0].first == vmVType::character){
                            for(int j = 0; j < list.size(); j++){
                                if(list[j].first != list[j - 1].first && j > 0) // 当前后类型不一致时，报错
                                    throw yoexception::YoError("TypeError", "All elements in the list must be of the same type", codes[i].line, codes[i].column);
                                newlist.push_back(Space::Scope::Value(constpool[list[j].second][0], constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                            }
                        }
                        runtimeSpace.createValue(name, Space::Scope::Value(newlist, constpool[codes[i].arg2] == "var"?false:true, codes[i].line, codes[i].column));
                    }
                }
                break;
            }
            case ygen::btc::scopestart: {
                runtimeSpace.createScope("__scope" + std::to_string(runtimeSpace.getDeep() + 1) + "__");
                break;
            }
            case ygen::btc::scopeend: {
                runtimeSpace.removeScope();
                break;
            }
            case ygen::btc::del: {
                if(codes[i].arg2 == 1.0) {
                    // 判断是否启用了不传入push名称的模式
                    auto name = constpool[codes[i].arg1];
                    if(runtimeSpace.find(name)) {
                        runtimeSpace.deleteValue(name);
                    }
                    else
                        throw yoexception::YoError("NameError", "There is no identifier named: '" + name + "'", codes[i].line, codes[i].column);
                }
                else {
                    auto name = constpool[envPop().second];
                    if(runtimeSpace.find(name)) {
                        runtimeSpace.deleteValue(name);
                    }
                    else
                        throw yoexception::YoError("NameError", "There is no identifier named: '" + name + "'", codes[i].line, codes[i].column);
                }
            }
            default: break;
        }
    }
    if(arg == "repl") std::cout<<std::endl;
    return 0;
}
void yvm::YVM::reload(std::vector<ygen::byteCode> _codes, std::vector<std::string> _constpool){
    codes = _codes;
    constpool = _constpool;
}
void yvm::YVM::clear() {
    runtimeSpace = Space();
    runtimeStack.clear();
    runtimeTop = 0;
    constpool.clear();
    codes.clear();
}
void yvm::YVM::loadVMFile(std::string path){
    std::ifstream file(path);
    std::istreambuf_iterator<char> begin(file);
    std::istreambuf_iterator<char> end;
    std::string content = std::string(begin, end);
    // Filter
    std::string str;
    for(int i = 0; i < content.size(); i++){
        if(content[i] == '\n'); // 过滤回车符
        else str.push_back(content[i]);
    }
    //
    auto ins = split(split(str, '#')[0], ':');
    auto cp = split(split(str, '#')[1], '\"');
    constpool = cp;

    // 生成code
    std::vector<ygen::byteCode> _codes;
    for(int i = 0; i < ins.size(); i ++){
        auto temp = split(ins[i], ',');
        _codes.push_back((ygen::byteCode){(ygen::btc)atoi(temp[0].c_str()),
                                          static_cast<float>(atof(temp[1].c_str())),
                                          static_cast<float>(atof(temp[2].c_str())),
                                          static_cast<float>(atof(temp[3].c_str())),
                                          static_cast<float>(atof(temp[4].c_str())),
                                          atoi(temp[5].c_str()), atoi(temp[6].c_str())});
    }
    codes = _codes;
}