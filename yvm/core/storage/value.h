#pragma once
#include "../type/YBoolean.h"
#include "../type/YString.h"
#include "../type/YDecimal.h"
#include "../type/YInteger.h"
#include "../type/YObject.h"

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
        Value(ytype::YInteger v, bool isc);
        Value(ytype::YBoolean v, bool isc);
        Value(ytype::YString v, bool isc);
        Value(ytype::YDecimal v, bool isc);
        Value(ytype::YObject v, bool isc);
        Value(std::vector<Value> v, bool isc);

        ygen::type::vtypeUnit getType(); // 获取Valu对应的完整Type

        ytype::YObject getObjectValue();
        ytype::YInteger getIntegerValue();
        ytype::YDecimal getDecimalValue();
        ytype::YBoolean getBooleanValue();
        ytype::YString getStringValue();

        std::vector<Value> getList();

        void setObjectValue(const ytype::YObject& v);
        void setIntegerValue(const ytype::YInteger& v);
        void setDecimalValue(const ytype::YDecimal& v);
        void setBooleanValue(const ytype::YBoolean& v);
        void setStringValue(const ytype::YString& v);

        void setObjectValueInList(const ytype::YObject& v, int pos);
        void setIntegerValueInList(const ytype::YInteger& v, int pos);
        void setDecimalValueInList(const ytype::YDecimal& v, int pos);
        void setBooleanValueInList(const ytype::YBoolean& v, int pos);
        void setStringValueInList(const ytype::YString& v, int pos);

        void coverList(std::vector<Value> ls);
    };
}