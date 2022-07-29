#include "space.h"

void ysto::Space::createValue(std::string name, ysto::Value v) {
    // createValue函数只会在当前Scope中创建新的值
    scopePool[deepCount].createValue(name, v);
}

bool ysto::Space::findValue(std::string name) {
    if(scopePool[deepCount].find(name)) return true;
    int temp = deepCount; // temp寄存
    while(temp != -1) {
        if(scopePool[temp].find(name)) return true;
        temp --;
    }
    return false;
}

ysto::Value &ysto::Space::getValue(std::string name) {
    if(scopePool[deepCount].find(name)) return scopePool[deepCount].get(name);
    int temp = deepCount; // temp寄存
    while(temp != -1) {
        if(scopePool[temp].find(name)) return scopePool[temp].get(name);
        temp --;
    }
    throw yoexception::YoError("NameError","The value of the identifier named '" + name + "' does not exist in '" + scopePool[deepCount].getIdentifier() + "'",
                               scopePool[deepCount].line,
                               scopePool[deepCount].column);
}

void ysto::Space::deleteValue(std::string name) {
    if(scopePool[deepCount].find(name)) scopePool[deepCount].deleteValue(name);
    int temp = deepCount; // temp寄存
    while(temp != -1) {
        if(scopePool[temp].find(name)) scopePool[temp].deleteValue(name);
        temp --;
    }
    throw yoexception::YoError("NameError","The value of the identifier named '" + name + "' does not exist in '" + scopePool[deepCount].getIdentifier() + "'",
                               scopePool[deepCount].line,
                               scopePool[deepCount].column);
}

void ysto::Space::createScope(std::string name, int ln, int col) {
    scopePool.push_back(Scope(name, ln, col));
    deepCount ++;
}

void ysto::Space::deleteScope() {
    if(deepCount == 0);
    else {
        scopePool.erase(scopePool.end());
        deepCount --;
    }
}

ysto::Scope &ysto::Space::getCurrent() {
    return scopePool[deepCount];
}
