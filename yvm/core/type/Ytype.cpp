#include "Ytype.h"

ytype::ytypeUnit ytype::type(ytype::basicType bt_, ytype::compType ct_) {
    return ytype::ytypeUnit{bt_, ct_};
}

std::string ytype::basicType2String(basicType bt) {
    switch (bt) {
        case integer:
            return "integer";
        case boolean:
            return "boolean";
        case decimal:
            return "decimal";
        case string:
            return "string";
        case null:
            return "null";
        case object:
            return "object";
        case iden:
            return "iden";
        case flag:
            return "flag";
        default: return "__null__";
    }
}
std::string ytype::compType2String(compType ct) {
    switch (ct) {
        case norm:
            return "norm";
        case list:
            return "list";
        case dict:
            return "dict";
        default:
            return "__null__";
    }
}
std::string ytype::type2String(ytypeUnit tu) {
    return basicType2String(tu.bt) + ":" + compType2String(tu.ct);
}

ytype::basicType ytype::string2BasicType(std::string s) {
    if(s=="integer") return basicType::integer;
    else if(s=="decimal") return basicType::decimal;
    else if(s=="boolean") return basicType::boolean;
    else if(s=="string") return basicType::string;
    else if(s=="null") return basicType::null;
}
ytype::compType ytype::string2CompType(std::string s) {
    if(s=="norm") return compType::norm;
    else if(s=="list") return compType::list;
    else if(s=="dict") return compType::dict;
}
ytype::ytypeUnit ytype::string2Type(std::string s) {
    std::string basic;
    std::string comp;
    for(int i = 0; i < s.size(); i++) {
        if(s[i]==':') break;
        basic += s[i];
    }
    bool flag = false;
    for(int i = 0; i < s.size(); i++) {
        if(s[i]==':') flag = true;
        if(flag) comp += s[i];
    }
    return {string2BasicType(basic), string2CompType(comp)};
}

bool ytype::ytypeUnit::operator==(ytypeUnit tu) {
    return this->bt == tu.bt && this->ct == tu.ct;
}
bool ytype::ytypeUnit::operator!=(ytypeUnit tu) {
    return this->bt != tu.bt && this->ct != tu.ct;
}

//

ytype::YObject::YObject(ytype::objectKind k) {
    kind = k;
}

ytype::YObject::YObject(std::vector<byteCode> cs, std::vector<std::pair<ytype::ytypeUnit, std::string>> as, ytype::ytypeUnit rety) {
    codes = cs;
    retType = rety;
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
