#include "../parser/parser.h"
#include <fstream>
#include <sstream>
#include <cstring>

namespace ygen{
    // ByteCode 集合
    enum btc{
        // 栈操作类
        push = 0, gto, jmp,
        // 运算类
        selfadd, selfsub, add, sub, div, mul, tmo, idx, lst, logicand, logicor, no, lt, ltet, gt, gtet, equ, noequ, gmem, stf, 
        // 标志类
        listend, paraend, scopestart, scopeend, idenend,
        // 功能类
        out, define, init, assign, del, call
    };

    namespace type{
        enum vtype {integer = 1, boolean, decimal, string, null, object, iden, flag}; // iden类型只是为了标记为标识符所代表的值
        typedef float modifier;
        typedef float vtypeUnit;
        extern modifier norm;
        extern modifier list;
        extern modifier dict;

        // 新类型系统的类型构造器
        float type(vtype t, modifier m);
        int getType(vtypeUnit unit);
        float getModifier(vtypeUnit unit);

        std::string vtype2String(vtype t);
        std::string modifier2String(modifier m);

        vtype string2Vtype(std::string s);
        modifier string2Modifier(std::string s);
    }

    // ByteCode的参数帮助，可以快捷的写一些参数。内部均为枚举，以XXXXHelper的形式存在
    namespace paraHelper{
        // reqXXX代表jmp指令需要的条件以供跳转
        // outWscope指的是跳出while所属的scope外并向前偏移，如果没有则向前偏移一个单位
        // findSStart代表跳转到所属scope的scopestart处
        enum jmpt{ reqTrue, reqFalse, findSStart, outWscope, outIFscope, outElifscope, outElsescope, outScopeDirectly};
    }

    /**
     * @brief 描述VM每一个code的结构体
     */
    struct byteCode{
        btc code;
        float arg1=0.0, arg2=0.0, arg3=0.0, arg4=0.0;
        int line, column;
    };

    class ByteCodeGenerator{
        std::vector<AST::StmtNode*> stmts;
        AST::WholeExprNode* expr;
        std::vector<std::string> constpool; // 字符（串）池，根据索引取元素(也可以用来存放参数)
        std::vector<byteCode> codes; // 字节码集
        int repit = -1; // 累计repit的计数器，起始值为-1

        std::string removeZero(float content); // 功能性函数，去除字符串末尾的0
    public:
        ByteCodeGenerator(std::vector<AST::StmtNode*> _stmts);
        ByteCodeGenerator(AST::WholeExprNode* _expr);

        std::vector<std::string> getConstPool(); // 获取字符（串）池
        std::vector<byteCode> getCodes(); // 获取字节码集
        int addPara(std::string para); // 在paras中加一个参数，并返回它所处的位置
        /**
         * @brief 最简ByteCode构造器，构造之后自动push进codes里面
         *
         * @param code ByteCode名称
         * @return byteCode
         */
        void minCtor(btc code, int ln, int col);
        /**
         * @brief 中型ByteCode构造器，一般会使用这个，构造之后自动push进codes里面
         *
         * @param code ByteCode名称
         * @param arg1 参数1（不可省略）
         * @param arg2 参数2（不可省略）
         * @return byteCode
         */
        void normalCtor(btc code, float arg1, float arg2, int ln, int col);
        /**
         * @brief 完整ByteCode构造器，构造之后自动push进codes里面
         *
         * @param code ByteCode名称
         * @param arg1 参数1（不可省略）
         * @param arg2 参数2（不可省略）
         * @param arg3 参数3（不可省略）
         * @param arg4 参数4（不可省略）
         * @return byteCode
         */
        void completeCtor(btc code, float arg1, float arg2, float arg3, float arg4, int ln, int col);
        // 用来生成 .ybtc 文件（yolang字节码文件）
        void genFile(std::string name);

        void visitNumber(yolexer::yoToken* token);
        void buildIntegerNumber(std::string number, int line, int column);
        void buildDecimalNumber(std::string number, int line, int column);
        void visitString(yolexer::yoToken* token);
        void visitNull(yolexer::yoToken* token);
        void visitBoolean(yolexer::yoToken* token);
        void visitIdentifier(AST::IdentifierNode* node);
        void visitIdentifierText(AST::IdentifierNode* node, bool isref = false);
        void visitSiadExpr(AST::SiadExprNode* node);
        void visitAddOp(AST::AddOpNode* node);
        void visitMulOp(AST::MulOpNode* node);
        void visitCmpOp(AST::CmpOpNode* node);
        void visitBoolOp(AST::BoolOpNode* node);
        void visitIndexOp(AST::IndexOpNode* node);
        void visitStfOp(AST::StfOpNode* node);
        void visitPrimExpr(AST::PrimExprNode* node);
        void visitMulExpr(AST::MulExprNode* node);
        void visitAddExpr(AST::AddExprNode* node);
        void visitCmpExpr(AST::CmpExprNode* node);
        void visitBoolExpr(AST::BoolExprNode* node);
        void visitListExpr(AST::ListExprNode* node);
        void visitAssignmentExpr(AST::AssignmentExprNode* node);
        void visitFuncCallExpr(AST::FuncCallNode* node);
        void visitExpr(AST::WholeExprNode* node); // 特殊的visitor，visit的是整个Expr

        void visitOutStmt(AST::OutStmtNode* node);
        void visitVorcStmt(AST::VorcStmtNode* node);
        void visitSpExprStmt(AST::SpExprStmtNode* node);
        void visitBlockStmt(AST::BlockStmtNode* node);
        void visitWhileStmt(AST::WhileStmtNode* node);
        void visitIfStmt(AST::IfStmtNode* node);
        void visitElifStmt(AST::ElifStmtNode* node);
        void visitElseStmt(AST::ElseStmtNode* node);
        void visitRepeatStmt(AST::RepeatStmtNode* node);
        void visitDeleteStmt(AST::DeleteStmtNode* node);
        void visitForStmt(AST::ForStmtNode* node);
        void visitBreakStmt(AST::BreakStmtNode* node);
        void visitFuncDefStmt(AST::FuncDefStmtNode* node);
        void visit(std::vector<AST::StmtNode*> stmts); // 特殊的visitor，visit的是整个stmts
    };
}