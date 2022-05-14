#include "../bytecode/generator.h"
#include <map>
#include <type_traits>

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
                    Value(std::vector<Value> list);
                    // 构造列表，isc用来检测列表是不是常量                
                    Value(std::vector<Value> list, bool isc);

                    bool isList();   // 判断当前Value是否为列表
                    bool isConst();  // 判断当前Value是否为Constant
                    ygen::paraHelper::type getType(); // 获得Value的type

                    template<class Type>
                    Type getValue();

                    template<class Type>
                    void assignValue(Type val);
                    void assignValue(std::string name, std::vector<Value> value);
                };
                friend Value;
                friend Space;
                typedef std::pair<std::string, Value> storage;
            private:
                std::vector<storage> values;
                std::vector<std::string> memberlist; // 成员名列表
                std::string idenname; // scope的标识名
            public:
                Scope(std::string name);
                // 查找是否有名为name的identifier
                bool findV(std::string name);
                // 返回名为name的value
                Value val(std::string name); 
                // 赋值
                template<class Type>
                void assign(std::string name, Type value);  
                // 获得名为name的value的position
                int pos(std::string name); 
                // 删除名为name的value
                void remove(std::string name); 

                std::string getName();
            };
            friend Scope;
        private:
            std::vector<Scope> scopestack; // scope栈
            int deepcount; // 深度计数器
        public:
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

            // 查找所有的scope（从当前scope一直回溯到globalScope）中是否有以“name”为名称的value
            bool find(std::string name);
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
        enum vmVType {iden, iden_text, string, boolean, character, integer, decimal, null}; // 必须与generator中的保持一致
    private:
        // 描述一个YVM中的Value
        typedef std::pair<vmVType, float> vmValue;
        
        std::string version = "v 0.0.1"; // 版本定义
        std::vector<ygen::byteCode> codes;
        std::vector<std::string> constpool;

        // 运行环境
        std::vector<vmValue> runtimeStack;
        int runtimeTop = 0;

        int addString(std::string str);
    public:
        // 对每一个vm实例的初始化
        YVM(ygen::ByteCodeGenerator bcg);

        std::string getVersion();
        void envPush(vmValue val);
        vmValue envPop();
        vmValue envPeek();
        void envClear();
        std::vector<std::string> getPool();

        int run(std::string arg);
    };
}