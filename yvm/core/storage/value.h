#pragma once
#include "../../../bytecode/generator.h"

namespace ysto {
    class Value {
        ytype::YObject objectValue;
        ytype::YInteger integerValue;
        ytype::YDecimal decimalValue;
        ytype::YBoolean booleanValue;
        ytype::YString stringValue;
        ytype::YNull nullValue;
        std::vector<Value> list;
        // 属性

        ytype::ytypeUnit type; // 值类型（无修饰符）
    public:
        bool isConstant = false; // 是否为常量
        bool isList = false; // 是否为列表
        bool isDynamic = false; // 是否为可变类型
        int line, column; // 行，列
        Value(ytype::YInteger v, bool isc, int ln, int col);
        Value(ytype::YBoolean v, bool isc, int ln, int col);
        Value(ytype::YString v, bool isc, int ln, int col);
        Value(ytype::YDecimal v, bool isc, int ln, int col);
        Value(ytype::YObject v, bool isc, int ln, int col);
        Value(std::vector<Value> v, bool isc, int ln, int col);
        Value(int ln, int col); // 初始化null

        Value(ytype::YInteger v, bool isc, bool isdyn, int ln, int col);
        Value(ytype::YBoolean v, bool isc, bool isdyn, int ln, int col);
        Value(ytype::YString v, bool isc, bool isdyn, int ln, int col);
        Value(ytype::YDecimal v, bool isc, bool isdyn, int ln, int col);
        Value(ytype::YObject v, bool isc, bool isdyn, int ln, int col);
        Value(std::vector<Value> v, bool isc, bool isdyn, int ln, int col);
        Value(bool isdyn, int ln, int col); // 初始化null

        Value(std::string content); // 用于初始化flag类型的构造函数（flag只用于内部

        ytype::basicType& getBasicType(); // 获取Value对应的BasicType
        ytype::compType& getCompType(); // 获取Value对应的BasicType
        ytype::ytypeUnit& getType(); // 获取Value对应的BasicType

        bool isConst();
        bool isListValue();
        bool isDyn();

        ytype::YObject& getObjectValue();
        ytype::YInteger& getIntegerValue();
        ytype::YDecimal& getDecimalValue();
        ytype::YBoolean& getBooleanValue();
        ytype::YString& getStringValue();
        ytype::YNull& getNullValue();

        std::vector<Value>& getList();

        void operator=(Value value);
        Value &operator[](int index);
    };
}