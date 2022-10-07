#pragma once
#include "value.h"

namespace ysto {
    class Scope {
        std::string identifier;
        std::vector<std::string> childNames;
        std::vector<Value> child;

    public:
        int line, column;
        Scope(std::string iden, int ln, int col);

        bool find(std::string name);
        int findPosition(std::string name);
        Value& getV(std::string name);
        bool deleteValue(std::string name);
        void createValue(std::string name, Value v);
        std::string getIdentifier();
    };
}