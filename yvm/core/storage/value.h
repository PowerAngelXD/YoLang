#pragma once
#include "../../../bytecode/generator.h"
#include <map>

namespace ysto {
    class Value {
        ytype::YObject objectValue;
        ytype::YInteger integerValue;
        ytype::YDecimal decimalValue;
        ytype::YBoolean booleanValue;
        ytype::YString stringValue;
        ytype::YNull nullValue;
        Value* refValue = nullptr;
        std::vector<Value> list;
        std::map<std::string, Value> newedStruct;

        ytype::ytypeUnit type;
    public:
        typedef std::pair<std::string, Value> element;
        bool isConstant = false;
        bool isList = false;
        bool isDynamic = false;
        int line, column;
        Value(ytype::YInteger v, bool isc, int ln, int col);
        Value(ytype::YBoolean v, bool isc, int ln, int col);
        Value(ytype::YString v, bool isc, int ln, int col);
        Value(ytype::YDecimal v, bool isc, int ln, int col);
        Value(ytype::YObject v, bool isc, int ln, int col);
        Value(std::vector<Value> v, bool isc, int ln, int col, bool isstrt);
        Value(std::map<std::string, Value> newedstrt, bool isc, int ln, int col);
        Value(Value* ref, bool isc, int ln, int col);
        Value(int ln, int col);

        Value(ytype::YInteger v, bool isc, bool isdyn, int ln, int col);
        Value(ytype::YBoolean v, bool isc, bool isdyn, int ln, int col);
        Value(ytype::YString v, bool isc, bool isdyn, int ln, int col);
        Value(ytype::YDecimal v, bool isc, bool isdyn, int ln, int col);
        Value(ytype::YObject v, bool isc, bool isdyn, int ln, int col);
        Value(std::vector<Value> v, bool isc, bool isdyn, int ln, int col, bool isstrt);
        Value(Value* ref, bool isc, bool isdyn, int ln, int col);
        Value(bool isdyn, int ln, int col);

        Value(std::string content);
        Value(Value* v);

        ytype::basicType& getBasicType();
        ytype::compType& getCompType();
        ytype::ytypeUnit& getType();

        bool isConst();
        bool isListValue();
        bool isDyn();

        ytype::YObject& getObjectValue();
        ytype::YInteger& getIntegerValue();
        ytype::YDecimal& getDecimalValue();
        ytype::YBoolean& getBooleanValue();
        ytype::YString& getStringValue();
        ytype::YNull& getNullValue();
        Value* getRef(int index=-1);

        std::vector<Value>& getList();
        std::map<std::string, Value>& getStrt();

        bool hasKey(std::string key);
        Value* getMap(std::string key);

        void operator=(Value value);
        Value &operator[](int index);
    };

    void printValue(Value result, std::string mode = "repr");
}