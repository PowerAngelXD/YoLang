#pragma once
#include "scope.h"

namespace ysto {
    class Space {
        std::vector<Scope> scopePool;
        int deepCount = 0;
    public:
        Space();
        // 创建一个value
        void createValue(std::string name, Value v);
        // 查找指定value
        bool findValue(std::string name);
        // 获取指定value
        Value getValue(std::string name);
        // 删除指定value
        void deleteValue(std::string name);

        // 创建一个scope，深度+1
        void createScope(std::string name);
        // 删除一个scope，深度-1
        void deleteScope();
    };
}