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
                class Value{
                    int intvalue;   // 值
                    float decivalue;   // 值
                    std::string strvalue;   // 值
                    char charvalue;   // 值
                    bool boolvalue;   // 值
                    
                    std::vector<Value> list; // 列表

                    ygen::paraHelper::type type; // 类型
                    int line, column; // 行，列

                    bool isconst = false;
                    bool islist = false;
                public:
                    // 构造变量或者常量，isc用来检测是不是常量                 
                    Value(int val, bool isc, int ln, int col);
                    Value(float val, bool isc, int ln, int col);
                    Value(std::string val, bool isc, int ln, int col);
                    Value(char val, bool isc, int ln, int col);
                    Value(bool val, bool isc, int ln, int col);
                    // 构造列表
                    Value(std::vector<Value> list, int ln, int col);
                    // 构造列表，isc用来检测列表是不是常量                
                    Value(std::vector<Value> list, bool isc, int ln, int col);

                    bool isList();   // 判断当前Value是否为列表
                    bool isConst();  // 判断当前Value是否为Constant
                    ygen::paraHelper::type getType(); // 获得Value的type
                    int getIntValue();
                    std::vector<Value> getList();

                    float getDeciValue();
                    std::string getStrValue();
                    char getCharValue();
                    bool getBoolValue();

                    void assignInt(int value);
                    void assignDeci(float value);
                    void assignBool(bool value);
                    void assignString(std::string value);
                    void assignChar(char value);
                    void assignValue(Value value);
                    void assignListValue(std::string name, std::vector<Value> value);
                };
                friend Value;
                friend Space;
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
                void assign(std::string name, std::string value);  
                void assign(std::string name, std::vector<Value> value);  
                // 获得名为name的value的position
                int pos(std::string name); 
                // 删除名为name的value
                void remove(std::string name); 
                // 创建一个新的Value实例
                void create(std::string name, Value value);
            };
            friend Scope;
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

            // 获得当前对应的深度值
            int getDeep();
        };
    }
    /**
     * @brief Yolang的VM实例类
     */
    class YVM{
    public:
        // 值在YVM中的类型
        enum vmVType {iden, iden_text, string, boolean, character, integer, decimal, null, list, flag}; // 必须与generator中的保持一致
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
    public:
        // 对每一个vm实例的初始化
        YVM(ygen::ByteCodeGenerator bcg);
        YVM();

        std::string getVersion();
        void envPush(vmValue val);
        vmValue envPop();
        vmValue envPeek();
        void envClear();
        std::vector<std::string> getConstPool();
        std::vector<std::vector<vmValue>> getListPool();

        int run(std::string arg);
        void reload(std::vector<ygen::byteCode> _codes, std::vector<std::string> _constpool);
        void loadVMFile(std::string path);
    };
}