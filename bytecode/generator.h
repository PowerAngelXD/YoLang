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
        selfadd, selfsub, add, sub, div, mul, tmo, idx, lst, logicand, logicor, no, lt, ltet, gt, gtet, equ, noequ, gmem, 
        // 标志类
        listend, paraend, scopestart, scopeend, idenend, 
        // 功能类
        out, define, init, assign
    };

    // ByteCode的参数帮助，可以快捷的写一些参数。内部均为枚举，以XXXXHelper的形式存在
    namespace paraHelper{
        // 对于值类型的帮助，有yolang系统类型
        // 说明：iden和iden_text
        // iden：标识符和它对应的值
        // iden_text：标识符本身，纯文字
        enum type{ iden, iden_text, string, boolean, character, integer, decimal, null, list, flag};
    }

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
        void visitStrCh(yolexer::yoToken* token);
        void visitNull(yolexer::yoToken* token);
        void visitBoolean(yolexer::yoToken* token);
        void visitIdentifier(AST::IdentifierNode* node);
        void visitIdentifierText(AST::IdentifierNode* node);
        void visitSiadExpr(AST::SiadExprNode* node);
        void visitAddOp(AST::AddOpNode* node);
        void visitMulOp(AST::MulOpNode* node);
        void visitCmpOp(AST::CmpOpNode* node);
        void visitBoolOp(AST::BoolOpNode* node);
        void visitIndexOp(AST::IndexOpNode* node);
        void visitPrimExpr(AST::PrimExprNode* node);
        void visitMulExpr(AST::MulExprNode* node);
        void visitAddExpr(AST::AddExprNode* node);
        void visitCmpExpr(AST::CmpExprNode* node);
        void visitBoolExpr(AST::BoolExprNode* node);
        void visitListExpr(AST::ListExprNode* node);
        void visitAssignmentExpr(AST::AssignmentExprNode* node);
        void visitExpr(AST::WholeExprNode* node); // 特殊的visitor，visit的是整个Expr

        void visitOutStmt(AST::OutStmtNode* node);
        void visitVorcStmt(AST::VorcStmtNode* node);
        void visitSpExprStmt(AST::SpExprStmtNode* node);
        void visit(std::vector<AST::StmtNode*> stmts); // 特殊的visitor，visit的是整个stmts
    };
}