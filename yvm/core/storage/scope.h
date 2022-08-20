#pragma once
#include "value.h"

namespace ysto {
    class Scope {
        std::string identifier; // 标识名
        std::vector<std::string> childNames; // Scope下所有Value的名称列表，与Value实例列表对应
        std::vector<Value> child; // Value实例列表

    public:
        int line, column; // 行，列
        Scope(std::string iden, int ln, int col);

        bool find(std::string name);
        int findPosition(std::string name);
        Value& getV(std::string name);
        bool deleteValue(std::string name);
        void createValue(std::string name, Value v);
        std::string getIdentifier();
    };
}