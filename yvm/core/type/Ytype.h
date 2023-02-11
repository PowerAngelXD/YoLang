#pragma once
#include "../type/YInteger.h"
#include "../type/YDecimal.h"
#include "../type/YBoolean.h"
#include "../type/YString.h"
#include "../type/YNull.h"
#include <vector>

namespace ytype {
    // 对原先的类型系统作出修改
    enum basicType {integer = 0, boolean, decimal, string, null, object, iden, flag};
    enum compType {norm = 10, list, strt, llike_strt, ref};
    struct ytypeUnit{
        basicType bt;
        compType ct;
        bool operator==(ytypeUnit tu);
        bool operator!=(ytypeUnit tu);
    };
    ytypeUnit type(basicType bt_, compType ct_);

    std::string basicType2String(basicType bt);
    std::string compType2String(compType ct);
    std::string type2String(ytypeUnit tu);

    basicType string2BasicType(std::string s);
    compType string2CompType(std::string s);
    ytypeUnit string2Type(std::string s);
    // Object
    typedef std::pair<std::string, ytypeUnit> structMemberPair;
    enum objectKind {typable, function, structable};

    /**
     * @brief 描述VM每一个code的结构体
     */
    struct byteCode{
        int code;
        ytypeUnit type;
        float arg1=0.0, arg2=0.0, arg3=0.0, arg4=0.0;
        int line, column;
    };

    struct YStruct {
        std::string ident;
        std::vector<structMemberPair> pairs;
        std::vector<ytypeUnit> initList;

        YStruct(std::string id, std::vector<ytypeUnit> init);
    };

    typedef std::vector<std::pair<ytype::ytypeUnit, std::string>> FunctionParas;
    struct YFunction {
        std::string ident;
        FunctionParas args; // 参数类型:参数名 only "function"
        ytype::ytypeUnit retType; // 返回类型
        std::vector<byteCode> codes; // 储存的代码片段（包含scopestart scopeend） only "function"

        YFunction(std::string id, FunctionParas paras, ytype::ytypeUnit retType, std::vector<byteCode> codes);
    };

    struct YImplement {
        std::string ident;
        std::vector<YFunction> methods;

        YImplement(std::string id, std::vector<YFunction> ms);
    };

    // 更完善的结构体，指有了至少一个implement
    struct YCompStruct {
        std::string ident;
        YStruct stru;
        YImplement mainImpl; // 主impl，是对struct的独有方法实现
        std::vector<YImplement> impls; // 其他impl，是从其它virtual impl上实现的

        YCompStruct(std::string id, YStruct stru, YImplement mImpl);
        void addImpl(YImplement impl);
        bool findImpl(std::string id);
        YImplement getImpl(std::string id);
    };

    class YObject {
        objectKind kind; // Object的类型

    public:
        std::string fnName; // only "function"
        std::vector<structMemberPair> memberPairs; // only "structable" or "typable"
        std::vector<ytypeUnit> initList; // only "typable"
        std::vector<std::pair<ytype::ytypeUnit, std::string>> args; // 参数类型:参数名 only "function"
        ytype::ytypeUnit retType; // 返回类型
        std::vector<byteCode> codes; // 储存的代码片段（包含scopestart scopeend） only "function"

        YObject(objectKind k);
        YObject()=default;
        // 构造一个函数Object的方法
        YObject(std::vector<byteCode> cs, std::vector<std::pair<ytype::ytypeUnit, std::string>> as, ytype::ytypeUnit rety, std::string name);
        YObject(std::vector<structMemberPair> members); // 构造struct
        YObject(std::vector<structMemberPair> members, std::vector<ytypeUnit> initl); // 构造typable

        objectKind getKind();

        void loadInVM(); // 将代码片段加载进VM中
    };
}