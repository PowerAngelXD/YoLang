#include "../bytecode/generator.h"
#include <map>
#include <type_traits>
#include <algorithm>

namespace yvm{
    class YVM;

    /**
     * @brief Yolang中的Var，Scope，Space实现
     */
    namespace var {
        class Space{
        public:
            class Scope{
            public:
                enum objKind{funcObj, typableObj}; // Object的类别

                class Value;
                class Object {
                    int line=0, column=0;

                    objKind kind; // Object的类别
                    std::string objName; // Object名称，funcObj和typableObj都适用
                    std::vector<ygen::byteCode> codes;
                    std::vector<std::string> constpool;
                    std::string rettype;
                    Space* objSpace = nullptr; // Object专用Space
                    // Object参数对，first：参数类型，second：参数名称（仅限于funcObj）
                    std::vector<std::pair<ygen::paraHelper::type, std::string>> paras;
                public:
                    YVM* objVM = nullptr; // Object专用VM（仅限于funcObj）

                    Object()=default; // 为Value而生的默认构造函数
                    Object(std::string name, objKind k, int ln, int col); // 最简Object构造器
                    Object(std::string name, objKind k, std::vector<ygen::byteCode> c, std::vector<std::string> cp, int ln, int col); // 构造Object的同时传入codes
                    Object(std::string name, std::vector<std::pair<ygen::paraHelper::type, std::string>> p, int ln, int col); // 构造FunctionObject，无codes
                    Object(std::string name, std::vector<ygen::byteCode> c,
                           std::vector<std::string> cp,
                           std::vector<std::pair<ygen::paraHelper::type, std::string>> p, std::string ret,
                           int ln, int col); // 构造完整的FunctionObject
                    // 获取目标成员名称（仅限于typable）
                    Value getMember(std::string name);
                    // 运行Object（仅限于funcObj）
                    YVM call(std::vector<Value> actparas);
                    // 获得Object的返回值（仅限于funcObj）
                    std::string getRettype();
                    // 判断Object是什么类别的
                    bool isFuncObj();
                    bool isTypableObj();
                };
                class Value{
                    int intvalue;   // 值
                    float decivalue;   // 值
                    std::string strvalue;   // 值
                    char charvalue;   // 值
                    bool boolvalue;   // 值
                    Object objvalue; // 值
                    Value *ref = nullptr; // 引用
                    
                    std::vector<Value> list; // 列表

                    ygen::paraHelper::type type; // 类型
                    int line, column; // 行，列

                    bool isconst = false;
                    bool isref = false;
                    bool islist = false;
                    bool isobj = false;
                    bool isnull = false;
                public:
                    // 构造变量或者常量，isc用来检测是不是常量
                    Value(bool isc, int ln, int col); // 构造null值
                    Value(int val, bool isc, int ln, int col);
                    Value(float val, bool isc, int ln, int col);
                    Value(std::string val, bool isc, int ln, int col);
                    Value(char val, bool isc, int ln, int col);
                    Value(bool val, bool isc, int ln, int col);
                    Value(Value* _ref, bool isc, int ln, int col);
                    Value(Object obj, bool isc, int ln, int col);
                    // 构造列表
                    Value(std::vector<Value> list, int ln, int col);
                    // 构造列表，isc用来检测列表是不是常量                
                    Value(std::vector<Value> list, bool isc, int ln, int col);

                    Value()=default;

                    bool isList();   // 判断当前Value是否为列表
                    bool isConst();  // 判断当前Value是否为Constant
                    bool isRef();    // 判断当前Value是否为引用
                    bool isObj();    // 判断当前Value是否为Object
                    bool isNull();   // 判断当前Value是否为Null
                    void removeNull(); // 取消Value的null状态
                    ygen::paraHelper::type getType(); // 获得Value的type

                    std::vector<Value> getList();
                    int getIntValue();
                    float getDeciValue();
                    std::string getStrValue();
                    char getCharValue();
                    bool getBoolValue();
                    Object getObjectValue();
                    Value* getRef();
                    Value* getSelfRef();


                    void assignInt(int value);
                    void assignDeci(float value);
                    void assignBool(bool value);
                    void assignString(std::string value);
                    void assignChar(char value);
                    void assignValue(Value value);
                    void assignListValue(std::vector<Value> value);
                    void assignObject(Object value);
                    void refAnother(Value value); // 重新ref一个value

                    friend Scope;
                };
                friend Value;
                friend Space;
                friend Object;
                typedef std::pair<std::string, Value> storage;
            private:
                std::vector<storage> values;
            public:
                std::vector<std::string> memberlist; // 成员名列表
                std::string idenname; // scope的标识名
                Scope(std::string name);
                // 查找是否有名为name的identifier
                bool findV(std::string name);
                // 返回名为name的value
                Value val(std::string name); 
                // 赋值
                void assign(std::string name, int value);  
                void assign(std::string name, float value);  
                void assign(std::string name, bool value);  
                void assign(std::string name, char value);  
                void assign(std::string name, Value value);
                void assign(std::string name, std::string value);  
                void assign(std::string name, std::vector<Value> value);
                void assign(std::string name, Object value);
                // 获得名为name的value的position
                int pos(std::string name); 
                // 删除名为name的value
                void remove(std::string name); 
                // 创建一个新的Value实例
                void create(std::string name, Value value);
                // 删除指定名称的value的null状态
                void rmNull(std::string name);
                // 更改指定value的类型
                void rewriteType(std::string name, ygen::paraHelper::type t);
                // 更改指定Value的列表类型
                void changeList(std::string name);
            };
            friend Scope;
            friend Scope::Object;
        private:
            std::vector<Scope> scopestack; // scope栈
            int deepcount = 0; // 深度计数器
        public:
            // 初始化
            Space();

            // 查找所有的Scope中是否有以“name”为名称的value，并返回这个Scope
            Scope getScope(std::string name);
            // 查找所有的Scope中是否有以“name”为名称的value，并返回这个Scope对应的索引值
            int getScopePos(std::string name);
            // 创建一个新的Scope
            void createScope(std::string name);
            // 删除当前Scope，并返回到上一级Scope中
            void removeScope();
            // 将当前Scope保存，先删除当前Scope，再返回到上一级Scope中，最后返回删除的Scope
            Scope PopScope(std::string name);
            // 返回当前deepcount指向的scope
            Scope& current();

            // 查找所有的scope（从当前scope一直回溯到globalScope）中是否有以“name”为名称的value
            bool find(std::string name);
            // 在当前Scope下创建新的Value实例
            void createValue(std::string name, Scope::Value value);
            // 查找所有的Scope中是否有以“name”为名称的value，并返回这个value
            Scope::Value getValue(std::string name);
            // 查找所有的Scope中是否有以“name”为名称的value，并返回这个value指向的Value的地址
            Scope::Value* getValueRef(std::string name);
            // 查找所有的Scope中是否有以“name”为名称的value，并返回这个value的地址
            Scope::Value* getValueSelfRef(std::string name);
            // 查找所有的Scope中是否有以“name”为名称的value，并返回这个value对应的索引值
            int getValuePos(std::string name);
            // 给名为“name”的value重新赋值
            template<class Type>
            void assignValue(std::string name, Type value);
            // 给名为“name”的value重新赋一个列表
            template<class Type>
            void assignValue(std::string name, std::vector<Type> values);
            // 删除名为“name”的value
            void deleteValue(std::string name);
            // 移除名为“name”的value的null状态
            void rmValueNull(std::string name);
            // 改变指定标识符的列表类型
            void toggleList(std::string name);
            // 更改指定Value的类型
            void reWriteType(std::string name, ygen::paraHelper::type t);

            // 获得当前对应的深度值
            int getDeep();
        };
    }
    /**
     * @brief Yolang的VM实例类
     */
    class YVM{
    public:
        // 值在YVM中的类型, 必须与generator中的保持一致
        enum vmVType {iden, iden_text, string, boolean, character, integer, decimal, null, list, flag, ref, obj};
        // bif表
        std::vector<std::string> bifNames = {"print", "println", "input", "sys", "len", "toInt", "toStr",
                                             "toDeci", "toBool"};
        //
    private:
        // 描述一个YVM中的Value
        typedef std::pair<vmVType, float> vmValue;

        std::vector<ygen::byteCode> codes;
        std::vector<std::string> constpool;
        std::vector<std::vector<vmValue>> listpool;

        // 运行环境
        std::vector<vmValue> runtimeStack;
        var::Space runtimeSpace;
        int runtimeTop = 0;

        int addString(std::string str);
        int addChar(char ch);
        int addList(std::vector<vmValue> values);
        int addValueList(std::vector<var::Space::Scope::Value> list);

        std::vector<std::string> split(std::string str, char sp);

        // BIF实现区域
        // 所有BIF函数名称均会以bif开头
        vmValue bifPrint(std::vector<vmValue> paras, int line, int column);
        vmValue bifPrintLn(std::vector<vmValue> paras, int line, int column);
        vmValue bifLen(std::vector<vmValue> paras, int line, int column);
        vmValue bifSys(std::vector<vmValue> paras, int line, int column);
        vmValue bifInput(std::vector<vmValue> paras, int line, int column);
        vmValue bifToInteger(std::vector<vmValue> paras, int line, int column);
        vmValue bifToString(std::vector<vmValue> paras, int line, int column);
        vmValue bifToDecimal(std::vector<vmValue> paras, int line, int column);
        vmValue bifToBoolean(std::vector<vmValue> paras, int line, int column);
    public:
        // 对每一个vm实例的初始化
        YVM(ygen::ByteCodeGenerator bcg);
        YVM();

        void envPush(vmValue val);
        vmValue envPop();
        vmValue envPeek();
        std::vector<std::string> getConstPool();
        std::vector<std::vector<vmValue>> getListPool();

        ygen::paraHelper::type string2Type(std::string str);

        int run(std::string arg);
        void reload(std::vector<ygen::byteCode> _codes, std::vector<std::string> _constpool);
        void clear(); // 清空环境
        void loadVMFile(std::string path);

        friend var::Space::Scope::Object;
    };
}