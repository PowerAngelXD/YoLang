#include "YObject.h"

ytype::YObject::YObject(ytype::objectKind k) {
    kind = k;
}

ytype::YObject::YObject(std::vector<ygen::byteCode> cs, std::vector<std::pair<ygen::type::vtypeUnit, std::string>> as) {
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
