#pragma once
#include "../type/YObject.h"
#include "../type/YInteger.h"
#include "../type/YDecimal.h"
#include "../type/YBoolean.h"
#include "../type/YString.h"

namespace ysto {
    class Value {
        ytype::YObject objectValue;
        ytype::YInteger integerValue;
        ytype::YDecimal decimalValue;
        ytype::YBoolean booleanValue;
        ytype::YString stringValue;
        std::vector<Value> list;
        // 属性
        bool isConstant = false; // 是否为常量
        bool isList = false; // 是否为列表

        ygen::type::vtype type; // 值类型（无修饰符）
    public:
        int line, column; // 行，列
        Value(ytype::YInteger v, bool isc, int ln, int col);
        Value(ytype::YBoolean v, bool isc, int ln, int col);
        Value(ytype::YString v, bool isc, int ln, int col);
        Value(ytype::YDecimal v, bool isc, int ln, int col);
        Value(ytype::YObject v, bool isc, int ln, int col);
        Value(std::vector<Value> v, bool isc, int ln, int col);

        ygen::type::vtypeUnit getType(); // 获取Valu对应的完整Type

        ytype::YObject& getObjectValue();
        ytype::YInteger& getIntegerValue();
        ytype::YDecimal& getDecimalValue();
        ytype::YBoolean& getBooleanValue();
        ytype::YString& getStringValue();

        std::vector<Value>& getList();
    };
}