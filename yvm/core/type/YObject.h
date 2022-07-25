#pragma once
#include "../../../bytecode/generator.h"

namespace ytype {
    /*
     * YOVM中得类型的基类，其他类型将依此为基础
     */
    class YObject {
        std::vector<std::string> members; // Object的所有成员的名称列表
    };
}
