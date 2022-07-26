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

        std::vector<ytype::YObject> objectList;
        std::vector<ytype::YInteger> integerList;
        std::vector<ytype::YDecimal> decimalList;
        std::vector<ytype::YBoolean> booleanList;
        std::vector<ytype::YString> stringList;

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
        Value(std::vector<ytype::YInteger> v, bool isc);
        Value(std::vector<ytype::YBoolean> v, bool isc);
        Value(std::vector<ytype::YString> v, bool isc);
        Value(std::vector<ytype::YDecimal> v, bool isc);
        Value(std::vector<ytype::YObject> v, bool isc);

        ygen::type::vtypeUnit getType(); // 获取Valu对应的完整Type
        ytype::YObject getObjectValue();
        ytype::YInteger getIntegerValue();
        ytype::YDecimal getDecimalValue();
        ytype::YBoolean getBooleanValue();
        ytype::YString getStringValue();

        std::vector<ytype::YObject> getObjectList();
        std::vector<ytype::YInteger> getIntegerList();
        std::vector<ytype::YDecimal> getDecimalList();
        std::vector<ytype::YBoolean> getBooleanList();
        std::vector<ytype::YString> getStringList();
    };
}