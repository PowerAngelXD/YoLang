#pragma once
#include "scope.h"

namespace ysto {
    class Space {
        std::vector<Scope> scopePool;
        int deepCount = 0;
    public:
        Space();
        void createValue(std::string name, Value v);
        bool findValue(std::string name);
        Value& getValue(std::string name);
        void deleteValue(std::string name);

        void createScope(std::string name, int ln, int col);
        void deleteScope();
        Scope& getCurrent();
    };
}