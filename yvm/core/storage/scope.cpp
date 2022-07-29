#include "scope.h"

ysto::Scope::Scope(std::string iden, int ln, int col): identifier(iden), column(col), line(ln) {}

bool ysto::Scope::find(std::string name) {
    for(int i = 0; i < childNames.size(); i ++) {
        if(name == childNames[i]) return true;
    }
    return false;
}

int ysto::Scope::findPosition(std::string name) {
    for(int i = 0; i < childNames.size(); i ++) {
        if(name == childNames[i]) return i;
    }
    return -1;
}

ysto::Value ysto::Scope::get(std::string name) {
    for(int i = 0; i < childNames.size(); i ++) {
        if(name == childNames[i]) return child[i];
    }
    throw yoexception::YoError("NameError","The value of the identifier named '" + name + "' does not exist in '" + identifier + "'", line, column);
}

void ysto::Scope::deleteValue(std::string name) {
    for(int i = 0; i < childNames.size(); i ++) {
        if(name == childNames[i]) {
            childNames.erase(childNames.begin() + i);
            child.erase(child.begin() + i);
        }
    }
    throw yoexception::YoError("NameError","The value of the identifier named '" + name + "' does not exist in '" + identifier + "'", line, column);
}

void ysto::Scope::createValue(std::string name, ysto::Value v) {
    childNames.push_back(name);
    child.push_back(v);
}

std::string ysto::Scope::getIdentifier() {
    return identifier;
}
