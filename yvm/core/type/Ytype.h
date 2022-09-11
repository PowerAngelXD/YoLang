#pragma once
#include "../type/YInteger.h"
#include "../type/YDecimal.h"
#include "../type/YBoolean.h"
#include "../type/YString.h"
#include "../type/YNull.h"
#include <vector>

namespace ytype {
    enum vtype {integer = 1, boolean, decimal, string, null, object, iden, flag}; // iden类型只是为了标记为标识符所代表的值
    typedef float modifier;
    typedef float vtypeUnit;

    // 新类型系统的类型构造器
    float type(vtype t, modifier m);
    int getType(vtypeUnit unit);
    float getModifier(vtypeUnit unit);

    std::string vtype2String(vtype t);
    std::string modifier2String(modifier m);

    vtype string2Vtype(std::string s);
    modifier string2Modifier(std::string s);

    //
    extern modifier norm;
    extern modifier list;
    extern modifier dict;

    //类型常量
    extern const vtypeUnit Integer;
    extern const vtypeUnit Decimal;
    extern const vtypeUnit String;
    extern const vtypeUnit Boolean;
    extern const vtypeUnit Object;


    // Object

    enum objectKind {typable, function};

    /**
     * @brief 描述VM每一个code的结构体
     */
    struct byteCode{
        int code;
        float arg1=0.0, arg2=0.0, arg3=0.0, arg4=0.0;
        int line, column;
    };

    class YObject {
        objectKind kind; // Object的类型

        std::vector<std::string> members; // only "typable"
        std::vector<std::pair<ytype::vtypeUnit, std::string>> args; // 参数类型:参数名 only "function"
        std::vector<byteCode> codes; // 储存的代码片段（包含scopestart scopeend） only "function"

    public:
        YObject(objectKind k);
        YObject()=default;
        // 构造一个函数Object的方法
        YObject(std::vector<byteCode> cs, std::vector<std::pair<ytype::vtypeUnit, std::string>> as);

        bool isTypable();
        bool isFunction();

        void loadInVM(); // 将代码片段加载进VM中
    };
}