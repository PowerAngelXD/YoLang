#include "value.h"

ysto::Value::Value(std::map<std::string, Value> newedstrt, bool isc, int ln, int col): newedStruct(newedstrt), isConstant(isc), line(ln), column(col), type({ytype::basicType::object, ytype::compType::strt}) {
    isDynamic = false;
}

ysto::Value::Value(ytype::YInteger v, bool isc, int ln, int col): integerValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::integer, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YBoolean v, bool isc, int ln, int col): booleanValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::boolean, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YString v, bool isc, int ln, int col): stringValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::string, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YDecimal v, bool isc, int ln, int col): decimalValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::decimal, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YObject v, bool isc, int ln, int col): objectValue(v), isConstant(isc), line(ln), column(col), type({ytype::basicType::object, ytype::compType::norm}) {}
ysto::Value::Value(ysto::Value *ref, bool isc, int ln, int col): refValue(ref), isConstant(isc), line(ln), column(col), type({ref->type.bt, ytype::compType::ref}) {}
ysto::Value::Value(int ln, int col) {
    type = {ytype::basicType::null, ytype::compType::norm};
    line = ln, column = col;
}
ysto::Value::Value(std::vector<Value> v, bool isc, int ln, int col, bool isstrt): list(v), isConstant(isc), line(ln), column(col), type({v[0].getType().bt, isstrt?ytype::compType::llike_strt:ytype::compType::list}), isList(isstrt?false:true) {}

ysto::Value::Value(ytype::YInteger v, bool isc, bool isdyn, int ln, int col): integerValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::integer, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YBoolean v, bool isc, bool isdyn, int ln, int col): booleanValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::boolean, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YString v, bool isc, bool isdyn, int ln, int col): stringValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::string, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YDecimal v, bool isc, bool isdyn, int ln, int col): decimalValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::decimal, ytype::compType::norm}) {}
ysto::Value::Value(ytype::YObject v, bool isc, bool isdyn, int ln, int col): objectValue(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ytype::basicType::object, ytype::compType::norm}) {}
ysto::Value::Value(ysto::Value *ref, bool isdyn, bool isc, int ln, int col): refValue(ref), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({ref->type.bt, ytype::compType::ref}) {}
ysto::Value::Value(bool isdyn, int ln, int col) {
    type = {ytype::basicType::null, ytype::compType::norm};
    isDynamic = isdyn;
    line = ln, column = col;
}
ysto::Value::Value(std::vector<Value> v, bool isc, bool isdyn, int ln, int col, bool isstrt): list(v), isDynamic(isdyn), isConstant(isc), line(ln), column(col), type({v[0].getType().bt, isstrt?ytype::compType::llike_strt:ytype::compType::list}), isList(isstrt?false:true) {}

ysto::Value::Value(std::string content) {
    stringValue = ytype::YString(content);
    type = {ytype::basicType::flag, ytype::compType::norm};
}
ysto::Value::Value(ysto::Value* v): type(ytype::type(v->type.bt, ytype::compType::ref)) {
    this->refValue = v;
    this->line = v->line;
    this->column = v->column;
}
ytype::basicType& ysto::Value::getBasicType() {
    return type.bt;
}
ytype::compType& ysto::Value::getCompType() {
    return type.ct;
}
ytype::ytypeUnit& ysto::Value::getType() {
    return type;
}

bool ysto::Value::isConst() {
    return isConstant;
}

bool ysto::Value::isListValue() {
    return isList;
}

bool ysto::Value::isDyn() {
    return isDynamic;
}

ytype::YObject &ysto::Value::getObjectValue() {
    return objectValue;
}

ytype::YInteger &ysto::Value::getIntegerValue() {
    return integerValue;
}

ytype::YDecimal &ysto::Value::getDecimalValue() {
    return decimalValue;
}

ytype::YBoolean &ysto::Value::getBooleanValue() {
    return booleanValue;
}

ytype::YString &ysto::Value::getStringValue() {
    return stringValue;
}

std::vector<ysto::Value> &ysto::Value::getList() {
    return list;
}

ytype::YNull &ysto::Value::getNullValue() {
    return nullValue;
}

void ysto::Value::operator=(ysto::Value value) {
    if(this->isConstant)
        throw yoexception::YoError("AssignError", "Constant cannot be assigned", line, column);
    if(this->getType().bt != value.getType().bt && !this->isDynamic)
        throw yoexception::YoError("AssignError", "You cannot assign a value of a different type to a variable of static type", this->line, this->column);
    this->type = value.type;
    if(this->type.ct == ytype::compType::list) {
        if(value.getType() != this->type)
            throw yoexception::YoError("AssignError", "You cannot assign a value of a different type to a variable or a constant", this->line, this->column);
        else list = value.getList();
    }
    else if(this->type.ct == ytype::compType::llike_strt) {
        list = value.getList();
    }
    else if(this->type.ct == ytype::compType::strt) {
        newedStruct = value.newedStruct;
    }
    else {
        switch (value.getType().bt) {
            case ytype::integer:
                this->integerValue = value.integerValue;
                break;
            case ytype::boolean:
                this->booleanValue = value.booleanValue;
                break;
            case ytype::decimal:
                this->decimalValue = value.decimalValue;
                break;
            case ytype::string:
                this->stringValue = value.stringValue;
                break;
            case ytype::null:
                this->nullValue = value.nullValue;
                break;
        }
    }
}

ysto::Value& ysto::Value::operator[](int index) {
    if(this->type.ct != ytype::compType::list && this->type.ct != ytype::compType::llike_strt)
        throw yoexception::YoError("TypeError", "The '[]' operator does not support operations on variables or constants that are not lists", this->line, this->column);
    return list[index];
}

bool ysto::Value::hasKey(std::string key) {
    for(auto e: newedStruct) {
        if(e.first == key) return true;
    }
    return false;
}

ysto::Value* ysto::Value::getMap(std::string key) {
    if(hasKey(key)) {
        return &newedStruct.at(key);
    }
    else
        throw yoexception::YoError("NameError", "Unknown member: '" + key + "'", line, column);
}

std::map<std::string, ysto::Value> &ysto::Value::getStrt() {
    return newedStruct;
}

ysto::Value *ysto::Value::getRef(int index) {
    if(index == -1)
        return refValue;
    else
        return &refValue->list[index];
}

void ysto::Value::refName(std::string name) {
    this->valueRefName = name;
}

std::string ysto::Value::getName() {
    return valueRefName;
}

//print
void ysto::printValue(Value result, std::string mode) {
    if(result.getCompType() == ytype::compType::ref) {
        printValue(*result.getRef(), mode);
    }
    else {
        if(result.isList) {
            std::cout<<"[";
            for(int i = 0; i < result.getList().size(); i ++) {
                auto elt = result.getList()[i];
                if(elt.getBasicType() == ytype::basicType::integer)
                    std::cout<<elt.getIntegerValue().get();
                else if(elt.getBasicType() == ytype::basicType::decimal)
                    std::cout<<elt.getDecimalValue().get();
                else if(elt.getBasicType() == ytype::basicType::string) {
                    if(mode == "repr") std::cout<<"\""<<elt.getStringValue().get()<<"\"";
                    else if(mode == "out") std::cout<<elt.getStringValue().get();
                }
                else if(elt.getBasicType() == ytype::basicType::boolean) {
                    if(elt.getBooleanValue().get())
                        std::cout<<"true";
                    else
                        std::cout<<"false";
                }
                else if(elt.getBasicType() == ytype::basicType::null) {
                    std::cout<<"<null>";
                }

                if(i != result.getList().size() - 1) {
                    std::cout<<", ";
                }
            }
            std::cout<<"]";
        }
        else if(result.getCompType() == ytype::compType::llike_strt) {
            std::cout<<"{";
            for(int i = 0; i < result.getList().size(); i ++) {
                auto elt = result.getList()[i];
                if(elt.getBasicType() == ytype::basicType::integer)
                    std::cout<<elt.getIntegerValue().get();
                else if(elt.getBasicType() == ytype::basicType::decimal)
                    std::cout<<elt.getDecimalValue().get();
                else if(elt.getBasicType() == ytype::basicType::string) {
                    if(mode == "repr") std::cout<<"\""<<elt.getStringValue().get()<<"\"";
                    else if(mode == "out") std::cout<<elt.getStringValue().get();
                }
                else if(elt.getBasicType() == ytype::basicType::boolean) {
                    if(elt.getBooleanValue().get())
                        std::cout<<"true";
                    else
                        std::cout<<"false";
                }
                else if(elt.getBasicType() == ytype::basicType::null) {
                    std::cout<<"<null>";
                }

                if(i != result.getList().size() - 1) {
                    std::cout<<", ";
                }
            }
            std::cout<<"}";
        }
        else{
            if(result.getBasicType() == ytype::basicType::integer)
                std::cout<<result.getIntegerValue().get()<<std::endl;
            else if(result.getBasicType() == ytype::basicType::decimal)
                std::cout<<result.getDecimalValue().get()<<std::endl;
            else if(result.getBasicType() == ytype::basicType::string) {
                if(mode == "repr") std::cout<<"\""<<result.getStringValue().get()<<"\"";
                else if(mode == "out") std::cout<<result.getStringValue().get();
            }
            else if(result.getBasicType() == ytype::basicType::boolean) {
                if(result.getBooleanValue().get())
                    std::cout<<"true"<<std::endl;
                else
                    std::cout<<"false"<<std::endl;
            }
            else if(result.getBasicType() == ytype::basicType::null) {
                std::cout<<"<null>"<<std::endl;
            }
        }
    }
}
