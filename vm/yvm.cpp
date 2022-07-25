#include "yvm.h"
using namespace yvm::var;
using namespace ygen;

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
void Space::reWriteType(std::string name, type::vtypeUnit t){
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
    memberlist.push_back(name);
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
void Space::Scope::assign(std::string name, std::string value) {
    values[pos(name)].second.assignString(value);
}
void Space::Scope::assign(std::string name, Value value) {
    values[pos(name)].second.refAnother(value);
}
void Space::Scope::assign(std::string name, std::vector<Value> value) {
    values[pos(name)].second.assignListValue(value);
}
void Space::Scope::rewriteType(std::string name, type::vtypeUnit t) {
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
Space::Scope::Object::Object(std::string name, std::vector<std::pair<type::vtypeUnit, std::string>> p, int ln, int col):
                            objName(name), paras(p), kind(objKind::funcObj), line(ln), column(col) {objVM = new yvm::YVM;}
Space::Scope::Object::Object(std::string name, std::vector<ygen::byteCode> c, std::vector<std::string> cp, std::vector<std::pair<type::vtypeUnit, std::string>> p, std::string ret, int ln, int col):
                            objName(name), paras(p), codes(c), constpool(cp), kind(objKind::funcObj), rettype(ret), line(ln), column(col) {objVM = new yvm::YVM;}

Space::Scope::Value Space::Scope::Object::getMember(std::string name) {
    if(!isTypableObj())
        throw yoexception::YoError("TypeError", "You cannot manipulate this Object as a class", line, column);
    if(objSpace->find(name))
        return objSpace->getValue(name);
    else
        throw yoexception::YoError("NameError", "Cannot find identifier named: '" + name + "'", line, column);
}

yvm::YVM Space::Scope::Object::call(std::vector<Value> actparas) {
    if(!isFuncObj())
        throw yoexception::YoError("TypeError", "You cannot call an Object of a non-function type as a function", line, column);
    // 创建临时参数变量
    for(int i = 0; i < paras.size(); i ++) {
        if(paras[i].first != actparas[i].getType()) {
            throw yoexception::YoError("TypeError", "The parameter '" + paras[i].second + "' passed in does not conform to the defined type", line, column);
        }
        objVM->runtimeSpace.createValue(paras[i].second, actparas[i]);
    }
    //
    objVM->reload(codes, constpool);
    objVM->run("null");
    return *objVM;
}

std::string Space::Scope::Object::getRettype() {return rettype;}

bool Space::Scope::Object::isFuncObj() {
    return kind == objKind::funcObj? true:false;
}
bool Space::Scope::Object::isTypableObj() {
    return kind == objKind::typableObj? true:false;
}
// Value
Space::Scope::Value::Value(bool isc, int ln, int col):
        isnull(true), isconst(isc), type(type::type(type::vtype::null, type::norm)), line(ln), column(col) {}
Space::Scope::Value::Value(int val, bool isc, int ln, int col): 
                intvalue(val), isconst(isc), type(type::type(type::vtype::integer, type::norm)), line(ln), column(col) {}
Space::Scope::Value::Value(float val, bool isc, int ln, int col): 
                decivalue(val), isconst(isc), type(type::type(type::vtype::decimal, type::norm)), line(ln), column(col) {}
Space::Scope::Value::Value(std::string val, bool isc, int ln, int col): 
                strvalue(val), isconst(isc), type(type::type(type::vtype::string, type::norm)), line(ln), column(col) {}
Space::Scope::Value::Value(bool val, bool isc, int ln, int col):
                boolvalue(val), isconst(isc), type(type::type(type::vtype::boolean, type::norm)), line(ln), column(col) {}
Space::Scope::Value::Value(Object val, bool isc, int ln, int col):
                objvalue(val), isconst(isc), type(type::type(type::vtype::object, type::norm)), line(ln), column(col), isobj(true) {}

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
type::vtypeUnit Space::Scope::Value::getType(){
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
void Space::Scope::Value::refAnother(Value value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    ref = &value;
}
void Space::Scope::Value::assignValue(Value value) {
    if(isconst) throw yoexception::YoError("ConstantError", "A constant cannot be assigned", line, column);
    this->boolvalue = value.boolvalue;
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
    if(list[0].getType() == type::vtype::integer){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue(list[i].getType(), list[i].getIntValue()));
        }
    }
    else if(list[0].getType() == type::vtype::decimal){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue(list[i].getType(), list[i].getDeciValue()));
        }
    }
    else if(list[0].getType() == type::vtype::boolean){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue(list[i].getType(), list[i].getBoolValue()));
        }
    }
    else if(list[0].getType() == type::vtype::string){
        for(int i = 0; i < list.size(); i++){
            newlist.push_back(vmValue(list[i].getType(), addString(list[i].getStrValue())));
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
            runtimeStack.emplace_back(vmValue(type::type(type::vtype::null, type::norm), 0.0));
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
    if(content.first == type::vtype::string)
        std::cout<<constpool[content.second];
    else if(type::getModifier(content.first) == type::list){
        auto list = listpool[content.second];
        for(int j = 0; j < list.size(); j++){
            auto elt = list[j];
            if(elt.first == type::vtype::string)
                std::cout<<constpool[elt.second];
            else
                std::cout<<elt.second;
        }
    }
    else
        std::cout<<content.second;
    return vmValue(type::type(type::vtype::null, type::norm), 0.0);
}
yvm::YVM::vmValue yvm::YVM::bifPrintLn(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    auto content = paras[0];
    if(content.first == type::vtype::string)
        std::cout<<constpool[content.second];
    else if(type::getModifier(content.first) == type::list){
        auto list = listpool[content.second];
        for(int j = 0; j < list.size(); j++){
            auto elt = list[j];
            if(elt.first == type::vtype::string)
                std::cout<<constpool[elt.second];
            else
                std::cout<<elt.second;
        }
    }
    else
        std::cout<<content.second;
    std::cout<<std::endl;
    return vmValue(type::type(type::vtype::null, type::norm), 0.0);
}
yvm::YVM::vmValue yvm::YVM::bifLen(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    if(type::getModifier(paras[0].first) != type::list && paras[0].first != type::vtype::string)
        throw yoexception::YoError("TypeError", "This function does not support arguments of this type", line, column);
    vmValue ret;
    if(type::getModifier(paras[0].first) == type::list)
        ret = vmValue(type::type(type::vtype::integer, type::norm), listpool[paras[0].second].size());
    else if(paras[0].first == type::vtype::string)
        ret = vmValue(type::type(type::vtype::integer, type::norm), constpool[paras[0].second].size());
    return ret;
}
yvm::YVM::vmValue yvm::YVM::bifSys(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    if(paras[0].first != type::vtype::string)
        throw yoexception::YoError("TypeError", "This function does not support arguments of this type", line, column);
    system(constpool[paras[0].second].c_str());
    return vmValue(type::type(type::vtype::null, type::norm), 0.0);
}
yvm::YVM::vmValue yvm::YVM::bifInput(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty() || paras.size() == 1) {
        if(paras.size() == 1)
            std::cout<<constpool[paras[0].second];
        std::string text;
        std::getline(std::cin, text);
        return vmValue(type::type(type::vtype::string, type::norm), addString(text));
    }
    else throw yoexception::YoError("ParaError", "Too many parameters", line, column);
}
yvm::YVM::vmValue yvm::YVM::bifToInteger(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    vmValue value;
    if(paras[0].first == type::vtype::integer) value = vmValue(type::type(type::vtype::integer, type::norm), (int)paras[0].second);
    else if(paras[0].first == type::vtype::string) value = vmValue(type::type(type::vtype::integer, type::norm), atoi(constpool[paras[0].second].c_str()));
    else if(paras[0].first == type::vtype::decimal) value = vmValue(type::type(type::vtype::integer, type::norm), static_cast<int>(paras[0].second));
    else if(paras[0].first == type::vtype::null) value = vmValue(type::type(type::vtype::integer, type::norm), 0);
    else if(paras[0].first == type::vtype::boolean) value = vmValue(type::type(type::vtype::integer, type::norm), (int)paras[0].second);
    return value;
}
yvm::YVM::vmValue yvm::YVM::bifToString(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    vmValue value;
    if(paras[0].first == type::vtype::integer) {
        std::ostringstream oss;
        oss << paras[0].second;
        value = vmValue(type::type(type::vtype::string, type::norm), addString(oss.str()));
    }
    else if(paras[0].first == type::vtype::string) value = vmValue(type::type(type::vtype::integer, type::norm), paras[0].second);
    else if(paras[0].first == type::vtype::decimal) {
        std::ostringstream oss;
        oss << paras[0].second;
        value = vmValue(type::type(type::vtype::string, type::norm), addString(oss.str()));
    }
    else if(paras[0].first == type::vtype::null) value = vmValue(type::type(type::vtype::integer, type::norm), addString("null"));
    else if(paras[0].first == type::vtype::boolean) {
        if((bool)paras[0].second == true) value = vmValue(type::type(type::vtype::integer, type::norm), addString("true"));
        else value = vmValue(type::type(type::vtype::integer, type::norm), addString("false"));
    }
    return value;
}
yvm::YVM::vmValue yvm::YVM::bifToDecimal(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    vmValue value;
    if(paras[0].first == type::vtype::integer) value = vmValue(type::type(type::vtype::decimal, type::norm), (int)paras[0].second);
    else if(paras[0].first == type::vtype::string) value = vmValue(type::type(type::vtype::decimal, type::norm), atof(constpool[paras[0].second].c_str()));
    else if(paras[0].first == type::vtype::decimal) value = vmValue(type::type(type::vtype::decimal, type::norm), static_cast<int>(paras[0].second));
    else if(paras[0].first == type::vtype::null) value = vmValue(type::type(type::vtype::decimal, type::norm), 0.0);
    else if(paras[0].first == type::vtype::boolean) value = vmValue(type::type(type::vtype::decimal, type::norm), (float)paras[0].second);
    return value;
}
yvm::YVM::vmValue yvm::YVM::bifToBoolean(std::vector<vmValue> paras, int line, int column) {
    if(paras.empty())
        throw yoexception::YoError("ParaError", "Too few parameters", line, column);
    else if(paras.size() > 1)
        throw yoexception::YoError("ParaError", "Too many parameters", line, column);
    vmValue value;
    if(paras[0].first == type::vtype::integer) value = vmValue(type::type(type::vtype::boolean, type::norm), (bool)paras[0].second);
    else if(paras[0].first == type::vtype::string) {
        if(constpool[paras[0].second] == "true") value = vmValue(type::type(type::vtype::boolean, type::norm), true);
        else value = vmValue(type::type(type::vtype::boolean, type::norm), false);
    }
    else if(paras[0].first == type::vtype::decimal) value = vmValue(type::type(type::vtype::boolean, type::norm), (bool)paras[0].second);
    else if(paras[0].first == type::vtype::null) value = vmValue(type::type(type::vtype::boolean, type::norm), false);
    else if(paras[0].first == type::vtype::boolean) value = vmValue(type::type(type::vtype::boolean, type::norm), (bool)paras[0].second);
    return value;
}
//

int yvm::YVM::run(std::string arg) {
    bool elseState = true;
    for(int i = 0; i < codes.size(); i++) {
        switch (codes[i].code)
        {
            case ygen::btc::push:{
                if(codes[i].arg2 == type::vtype::iden){
                    if(codes[i + 1].code == ygen::btc::idenend){
                        // 单个identifier，直接push
                        auto name = constpool[codes[i].arg1];
                        if(runtimeSpace.find(name)){
                            auto type = runtimeSpace.getValue(name).getType();
                            if(!runtimeSpace.getValue(name).isList()){
                                 if(type == type::type(type::vtype::null, type::norm)) envPush(vmValue(type::type(type::vtype::null, type::norm), 0));
                                 else if(type == type::type(type::vtype::integer, type::norm)) envPush(vmValue(type, runtimeSpace.getValue(name).getIntValue()));
                                 else if(type == type::type(type::vtype::decimal, type::norm)) envPush(vmValue(type, runtimeSpace.getValue(name).getDeciValue()));
                                 else if(type == type::type(type::vtype::boolean, type::norm)) envPush(vmValue(type, runtimeSpace.getValue(name).getBoolValue()));
                                 else if(type ==type::type(type::vtype::string, type::norm)) envPush(vmValue(type, addString(runtimeSpace.getValue(name).getStrValue())));
                            }
                            else{
                                envPush(vmValue(type::type((type::vtype)type::getType(type), type::list), addValueList(runtimeSpace.getValue(name).getList())));
                            }
                        }
                        else throw yoexception::YoError("NameError", "Cannot find identifier named: '" + constpool[codes[i].arg1] + "'", codes[i].line, codes[i].column);
                    }
                    else ; // 多个identifier连接在一起
                }
                else envPush(vmValue(type::type((type::vtype)codes[i].arg2, type::norm), codes[i].arg1));
                break;
            }
            case ygen::btc::call: {
                std::vector<vmValue> temp;
                bool hasPara = false;
                auto name = constpool[envPop().second]; // funcName
                if(envPeek().first == type::type(type::vtype::flag, type::norm) && constpool[envPeek().second] == "PARAENDFLAG")
                    hasPara = true;
                envPop(); // 删除flag
                if(hasPara) {
                    // 有参数，开始制作参数列表
                    while(envPeek().first != type::type(type::vtype::flag, type::norm) && !runtimeStack.empty()) {
                        temp.push_back(envPop());
                    }
                    envPop(); // 删除参数末尾的flag
                }
                std::reverse(temp.begin(), temp.end()); // 出来的参数顺序颠倒
                if(std::find(bifNames.begin(), bifNames.end(), name) != bifNames.end()) {
                    // 找到了bifname，是bif，交给bif处理
                    if(name == "print")
                        envPush(bifPrint(temp, codes[i].line, codes[i].column));
                    else if(name == "println")
                        envPush(bifPrintLn(temp, codes[i].line, codes[i].column));
                    else if(name == "len")
                        envPush(bifLen(temp, codes[i].line, codes[i].column));
                    else if(name == "sys")
                        envPush(bifSys(temp, codes[i].line, codes[i].column));
                    else if(name == "input")
                        envPush(bifInput(temp, codes[i].line, codes[i].column));
                    else if(name == "toInt")
                        envPush(bifToInteger(temp, codes[i].line, codes[i].column));
                    else if(name == "toStr")
                        envPush(bifToString(temp, codes[i].line, codes[i].column));
                    else if(name == "toDeci")
                        envPush(bifToDecimal(temp, codes[i].line, codes[i].column));
                    else if(name == "toBool")
                        envPush(bifToBoolean(temp, codes[i].line, codes[i].column));
                }
                else {
                    auto function = runtimeSpace.getValue(name);
                    function.getObjectValue().objVM->runtimeSpace = this->runtimeSpace;
                    // 转换成Value
                    std::vector<Space::Scope::Value> paras;
                    for(int j = 0; j < temp.size(); j ++) {
                        if(temp[i].first == type::type(type::vtype::integer, type::norm))
                                paras.push_back(Space::Scope::Value((int)temp[j].second, false, codes[i].line, codes[i].column));
                    }
                    //
                    if(function.isObj()) {
                        function.getObjectValue().call(paras);
                        if(function.getObjectValue().getRettype() == "null") {
                            envPush(vmValue(type::type(type::vtype::null, type::norm), 0.0));
                        }
                    }
                }
                break;
            }
            case ygen::btc::listend:{
                envPush(vmValue(type::type(type::vtype::flag, type::norm), addString("LISTENDFLAG")));
                break;
            }
            case ygen::btc::paraend:{
                envPush(vmValue(type::type(type::vtype::flag, type::norm), addString("PARAENDFLAG")));
                break;
            }
            case ygen::btc::lst:{
                std::vector<vmValue> list;
                bool flag = false;
                while(runtimeStack[runtimeTop].first != type::type(type::vtype::flag, type::norm)){
                    list.push_back(envPop());
                }
                list.erase(list.begin() + list.size() - 1);
                std::reverse(list.begin(), list.end());
                envPush(vmValue(type::type((type::vtype)type::getType(list[0].first), type::list), addList(list)));
                break;
            }
            case ygen::btc::idx:{
                auto index = envPop();
                auto list = envPop();
                if(type::getModifier(list.first) == type::list){
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
                if(runtimeSpace.getValue(constpool[iden.second]).getType() != type::type(type::vtype::integer, type::norm))
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column);
                
                if(isFront) {
                    auto temp = runtimeSpace.getValue(constpool[iden.second]).getIntValue() + 1;
                    runtimeSpace.assignValue(constpool[iden.second], 
                        temp);
                    envPush(vmValue(type::type(type::vtype::integer, type::norm), runtimeSpace.getValue(constpool[iden.second]).getIntValue()));
                }
                else {
                    auto temp = runtimeSpace.getValue(constpool[iden.second]).getIntValue() + 1;
                    envPush(vmValue(type::type(type::vtype::integer, type::norm), runtimeSpace.getValue(constpool[iden.second]).getIntValue()));
                    runtimeSpace.assignValue(constpool[iden.second], 
                        temp);
                }

                break;
            }
            case ygen::btc::selfsub:{
                auto iden = envPop();
                bool isFront = codes[i].arg1 == 1.0 ? true : false;
                if(runtimeSpace.getValue(constpool[iden.second]).getType() != type::type(type::vtype::integer, type::norm))
                    throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column);
                
                if(isFront) {
                    auto temp = runtimeSpace.getValue(constpool[iden.second]).getIntValue() - 1;
                    runtimeSpace.assignValue(constpool[iden.second], 
                        temp);
                    envPush(vmValue(type::type(type::vtype::integer, type::norm), runtimeSpace.getValue(constpool[iden.second]).getIntValue()));
                }
                else {
                    auto temp = runtimeSpace.getValue(constpool[iden.second]).getIntValue() - 1;
                    envPush(vmValue(type::type(type::vtype::integer, type::norm), runtimeSpace.getValue(constpool[iden.second]).getIntValue()));
                    runtimeSpace.assignValue(constpool[iden.second], 
                        temp);
                }

                break;
            }
            case ygen::btc::add:{
                auto right = envPop();
                auto left = envPop();
                if(left.first == type::type(type::vtype::integer, type::norm)) {
                    if(right.first == type::type(type::vtype::integer, type::norm)) {

                    }
                    else if(right.first == type::type(type::vtype::decimal, type::norm)) {

                    }
                    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column);
                }
                else if(left.first == type::type(type::vtype::decimal, type::norm)) {
                    if(right.first == type::type(type::vtype::integer, type::norm)) {

                    }
                    else if(right.first == type::type(type::vtype::decimal, type::norm)) {

                    }
                    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column);
                }
                else if(left.first == type::type(type::vtype::string, type::norm)) {
                    if(right.first == type::type(type::vtype::string, type::norm)) {

                    }
                    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",codes[i].line, codes[i].column);
                }
                break;
            }
            case ygen::btc::sub:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::div:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::mul:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::tmo:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::equ:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::noequ:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::gt:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::gtet:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::lt:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::ltet:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::no:{

                break;
            }
            case ygen::btc::logicand:{
                auto right = envPop();
                auto left = envPop();

                break;
            }
            case ygen::btc::logicor:{
                auto right = envPop();
                auto left = envPop();

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
                                if(envPeek().first == type::type(type::vtype::null, type::norm)) envPop(); // 排除潜在的null值
                                auto cond = envPop();
                                if((bool)cond.second == true) {
                                    envPush(vmValue(type::type(type::vtype::boolean, type::norm), 1.0));
                                    elseState = false;
                                } // 条件为真进入
                                else {
                                    int state = 0;
                                    while(true) {
                                        i ++;
                                        if(codes[i].code == ygen::scopestart) state ++;
                                        else if(codes[i].code == ygen::scopeend) state --;

                                        if(state == 0) break;
                                    }
                                    envPush(vmValue(type::type(type::vtype::boolean, type::norm), 0));
                                }
                                break;
                            }
                            case ygen::paraHelper::jmpt::outElifscope: {
                                auto cond = envPop();
                                if(envPeek().first == type::type(type::vtype::null, type::norm)) envPop(); // 排除潜在的null值
                                if(runtimeStack.empty()) 
                                    throw yoexception::YoError("Syntax", "You cannot use elif statements directly without an if statement", codes[i].line, codes[i].column);
                                else if(envPeek().first != type::type(type::vtype::boolean, type::norm))
                                    throw yoexception::YoError("Syntax", "You cannot use elif statements directly without an if statement", codes[i].line, codes[i].column);
                                if((bool)cond.second == true && (bool)envPop().second == false) {
                                    envPush(vmValue(type::type(type::vtype::boolean, type::norm), 1.0));
                                    elseState = false;
                                } // 条件为真进入
                                else {
                                    int state = 0;
                                    while(true) {
                                        i ++;
                                        if(codes[i].code == ygen::scopestart) state ++;
                                        else if(codes[i].code == ygen::scopeend) state --;

                                        if(state == 0) break;
                                    }
                                    envPush(vmValue(type::type(type::vtype::boolean, type::norm), 0));
                                }
                                break;
                            }
                            case ygen::paraHelper::jmpt::outElsescope: {
                                if(envPeek().first == type::type(type::vtype::null, type::norm)) envPop(); // 排除潜在的null值
                                if(runtimeStack.empty()) 
                                    throw yoexception::YoError("Syntax", "You cannot use else statements directly without an if statement", codes[i].line, codes[i].column);
                                else if(envPeek().first != type::type(type::vtype::boolean, type::norm))
                                    throw yoexception::YoError("Syntax", "You cannot use else statements directly without an if statement", codes[i].line, codes[i].column);
                                if((bool)envPop().second == false && elseState == true) envPush(vmValue(type::type(type::vtype::boolean, type::norm), 1.0)); // 条件为真进入
                                else {
                                    int state = 0;
                                    while(true) {
                                        i ++;
                                        if(codes[i].code == ygen::scopestart) state ++;
                                        else if(codes[i].code == ygen::scopeend) state --;

                                        if(state == 0) break;
                                    }
                                    envPush(vmValue(type::type(type::vtype::boolean, type::norm), 0));
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

                    }
                }
                break;
            }
            case ygen::btc::out:{

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
                if(value.first != runtimeSpace.getValue(constpool[name.second]).getType() &&
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
                    // TODO: 针对对应索引的元素的赋值
                    runtimeSpace.assignValue(constpool[name.second], list);
                }
                else {
                    // 普通赋值
                }
                envPush(vmValue(type::type(type::vtype::null, type::norm), 0.0));
                break;
            }     
            case ygen::btc::init:{
                auto name = constpool[codes[i].arg1];
                if(constpool[codes[i].arg2] == "function") {
                    int line = codes[i].line, column = codes[i].column;
                    std::vector<std::pair<type::vtypeUnit, std::string>> paras;
                    while(constpool[envPeek().second] != "PARAENDFLAG") {
                        std::pair<type::vtypeUnit , std::string> para;
                        auto pname = envPop();
                        auto ptype = envPop();
                        // para.first = string2Type(constpool[ptype.second]); // string2Type函数的重写
                        para.second = constpool[pname.second];
                        paras.push_back(para);
                    }
                    envPop(); // 跳过paraendflag
                    std::reverse(paras.begin(), paras.end()); // 参数顺序颠倒
                    auto rettype = constpool[envPop().second];
                    // 提取函数体内容
                    std::vector<ygen::byteCode> fcodes;
                    int state = 0;
                    while(true) {
                        i ++;
                        if(codes[i].code == ygen::scopestart) state ++;
                        else if(codes[i].code == ygen::scopeend) state --;
                        fcodes.push_back(codes[i]);
                        if(state == 0) break;
                    }
                    //
                    Space::Scope::Object function(name, fcodes, constpool, paras, rettype, line, column);
                    runtimeSpace.createValue(name, Space::Scope::Value(function, false, line, column));
                }
                else {
                    if(std::count(runtimeSpace.current().memberlist.begin(), runtimeSpace.current().memberlist.end(), name)){
                        auto value = envPop();
                        //type-checker
                        if(codes[i].arg4 == 1.0){
                            auto reqType = constpool[codes[i].arg3];
                            if(value.first == type::type(type::vtype::null, type::norm)){
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
                                // TODO：类型标注系统的重写
                            }
                        }
                        //TODO: 创建变量的逻辑重写
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