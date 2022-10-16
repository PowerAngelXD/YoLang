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
        case strt:
            return "strt";
        default:
            return "__null__";
    }
}
std::string ytype::type2String(ytypeUnit tu) {
    std::string ret = basicType2String(tu.bt);
    if(tu.ct == ytype::compType::norm || tu.ct == ytype::compType::ref);
    else if(tu.ct == ytype::compType::list) ret += "[]";
    else if(tu.ct == ytype::compType::strt) ret += "{=struct}";
    else if(tu.ct == ytype::compType::llike_strt) ret += "{}";
    return ret;
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
    else if(s=="strt") return compType::strt;
}
ytype::ytypeUnit ytype::string2Type(std::string s) {
    std::string basic;
    std::string comp;
    if(s[s.size()-1] == ']') {
        comp = "list";
        for(int i = 0; i < s.size(); i ++) {
            if(s[i] == '[') break;
            basic += s[i];
        }
        return {string2BasicType(basic), string2CompType(comp)};
    }
    if(s[s.size()-1] == '}') {
        comp = "strt";
        for(int i = 0; i < s.size(); i ++) {
            if(s[i] == '{') break;
            basic += s[i];
        }
        return {string2BasicType(basic), string2CompType(comp)};
    }
    else {
        return {string2BasicType(s), ytype::compType::norm};
    }
}

bool ytype::ytypeUnit::operator==(ytypeUnit tu) {
    return this->bt == tu.bt && this->ct == tu.ct;
}
bool ytype::ytypeUnit::operator!=(ytypeUnit tu) {
    return this->bt != tu.bt || this->ct != tu.ct;
}

//

ytype::YObject::YObject(ytype::objectKind k) {
    kind = k;
}

ytype::YObject::YObject(std::vector<byteCode> cs, std::vector<std::pair<ytype::ytypeUnit, std::string>> as, ytype::ytypeUnit rety, std::string name) {
    codes = cs;
    retType = rety;
    args = as;
    fnName = name;
    kind = objectKind::function;
}

ytype::YObject::YObject(std::vector<structMemberPair> members, std::vector<ytypeUnit> initl) {
    memberPairs = members;
    initList = initl;
    kind = objectKind::typable;
}

ytype::YObject::YObject(std::vector<structMemberPair> members) {
    kind = objectKind::structable;
    memberPairs = members;
}

void ytype::YObject::loadInVM() {

}

ytype::objectKind ytype::YObject::getKind() {
    return this->kind;
}
