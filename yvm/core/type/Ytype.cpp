#include "Ytype.h"

ytype::modifier ytype::norm = 0.1;
ytype::modifier ytype::list = 0.2;
ytype::modifier ytype::dict = 0.3;

const ytype::vtypeUnit ytype::Integer = ytype::type(ytype::vtype::integer,ytype::norm);
const ytype::vtypeUnit ytype::Decimal = ytype::type(ytype::vtype::decimal,ytype::norm);
const ytype::vtypeUnit ytype::String = ytype::type(ytype::vtype::string, ytype::norm);
const ytype::vtypeUnit ytype::Boolean = ytype::type(ytype::vtype::boolean,ytype::norm);
const ytype::vtypeUnit ytype::Object = ytype::type(ytype::vtype::object, ytype::norm);

float ytype::type(vtype t, modifier m) {
    return t + m;
}
int ytype::getType(vtypeUnit unit) {
    ytype::vtype t = static_cast<ytype::vtype>(unit);
    return t;
}
float ytype::getModifier(vtypeUnit unit) {
    ytype::vtype t = static_cast<ytype::vtype>(unit);
    ytype::modifier m = unit - t;
    return m;
}

ytype::vtype ytype::string2Vtype(std::string s) {
    ytype::vtype t;
    if(s == "integer") t = ytype::vtype::integer;
    else if(s == "decimal") t = ytype::vtype::decimal;
    else if(s == "string") t = ytype::vtype::string;
    else if(s == "object") t = ytype::vtype::object;
    else if(s == "boolean") t = ytype::vtype::boolean;
    else if(s == "null") t = ytype::vtype::null;
    return t;
}
ytype::modifier ytype::string2Modifier(std::string s) {
    modifier m;
    if(s == "norm") m = norm;
    else if(s == "list") m = list;
    else if(s == "dict") m = dict;
    return m;
}

std::string ytype::modifier2String(modifier m) {
    std::string ret;
    if(m == norm) ret = "norm";
    else if(m == list) ret = "list";
    else if(m == dict) ret = "dict";
    return ret;
}
std::string ytype::vtype2String(ytype::vtype t) {
    std::string ret = "null";
    switch (t) {
        case integer:
            ret = "integer";
            break;
        case boolean:
            ret = "boolean";
            break;
        case decimal:
            ret = "decimal";
            break;
        case string:
            ret = "string";
            break;
        case null:
            ret = "null";
            break;
        case object:
            ret = "obj";
            break;
        default:
            break;
    }
    return ret;
}


ytype::YObject::YObject(ytype::objectKind k) {
    kind = k;
}

ytype::YObject::YObject(std::vector<byteCode> cs, std::vector<std::pair<ytype::vtypeUnit, std::string>> as) {
    codes = cs;
    args = as;
    kind = objectKind::function;
}

bool ytype::YObject::isTypable() {
    return kind == objectKind::typable?true:false;
}

bool ytype::YObject::isFunction() {
    return kind == objectKind::function?true:false;
}

void ytype::YObject::loadInVM() {

}
