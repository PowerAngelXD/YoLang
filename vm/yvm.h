#include "../bytecode/generator.h"
#include <map>

namespace yvm{
    class YVM;

    /**
     * @brief Yolang中的Var，Scope，Space实现
     */
    namespace var {
        class Value{
            int intValue;
            std::vector<Value> list;
            ygen::paraHelper::typeHelper type;

            bool islist = false;
        public:
            bool isList();
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

        // 运行时栈
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