#pragma once
#include "../../../bytecode/generator.h"

namespace ytype {
    enum objectKind {typable, function};

    class YObject {
        objectKind kind; // Object的类型

        std::vector<std::string> members; // only "typable"
        std::vector<std::pair<ygen::type::vtypeUnit, std::string>> args; // 参数类型:参数名 only "function"
        std::vector<ygen::byteCode> codes; // 储存的代码片段（包含scopestart scopeend） only "function"

    public:
        YObject(objectKind k);
        // 构造一个函数Object的方法
        YObject(std::vector<ygen::byteCode> cs, std::vector<std::pair<ygen::type::vtypeUnit, std::string>> as);

        bool isTypable();
        bool isFunction();

        void loadInVM(); // 将代码片段加载进VM中
    };
}
