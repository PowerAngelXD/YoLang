#pragma once
#include "../lexer/lexer.h"

/**
 * @brief AST树
 */
namespace AST{
    class AddExprNode;
    class ListExprNode; class WholeExprNode;
    class StmtNode; class StfOpNode; class TypecastExprNode;
    class CellExprNode; class IdentifierExprNode;

    class IndexOpNode{
    public:
        yolexer::yoToken* left = nullptr;
        AddExprNode* index = nullptr;
        yolexer::yoToken* right = nullptr;

        std::string toString();
    };

    class CallOpNode{
    public:
        yolexer::yoToken* left = nullptr;
        std::vector<WholeExprNode*> paras;
        std::vector<yolexer::yoToken*> dots;
        yolexer::yoToken* right = nullptr;

        std::string toString();
    };


    class CellIdentifierNode {
    public:
        yolexer::yoToken* iden = nullptr;
        CallOpNode* call = nullptr;

        std::string toString();
    };

    class IdentifierNode {
    public:
        CellIdentifierNode* iden = nullptr;
        IndexOpNode* idx = nullptr;

        std::string toString();
    };

    class IdentifierExprNode{
    public:
        std::vector<IdentifierNode*> idens;
        std::vector<yolexer::yoToken*> dots;

        std::string toString();
    };

    class SiadExprNode{
    public:
        bool isFront = false; // 是否为前置
        IdentifierExprNode* iden = nullptr;
        yolexer::yoToken* op = nullptr;

        std::string toString();
    };

    class PrimExprNode{
    public:
        yolexer::yoToken* number = nullptr;
        yolexer::yoToken* string = nullptr;
        yolexer::yoToken* null = nullptr;
        yolexer::yoToken* boolconst = nullptr;
        IdentifierExprNode* iden = nullptr;
        StfOpNode* stf = nullptr;
        SiadExprNode* siad = nullptr;
        TypecastExprNode* typecast = nullptr;
        CellExprNode* cellexpr = nullptr;

        // 括号
        yolexer::yoToken* left = nullptr;
        WholeExprNode* expr = nullptr;
        yolexer::yoToken* right = nullptr;

        IndexOpNode* op = nullptr;

        std::string toString();
    };

    class CellExprNode{
    public:
        yolexer::yoToken* op = nullptr;
        PrimExprNode* expr = nullptr;

        std::string toString();
    };

    class AsOpNode {
    public:
        yolexer::yoToken* op = nullptr;

        std::string toString();
    };

    class AddOpNode{
    public:
        yolexer::yoToken* op = nullptr;

        std::string toString();
    };

    class MulOpNode{
    public:
        yolexer::yoToken* op = nullptr;

        std::string toString();
    };

    class MulExprNode{
    public:
        std::vector<PrimExprNode*> prims;
        std::vector<MulOpNode*> ops;

        std::string toString();
    };

    class AddExprNode{
    public:
        std::vector<MulExprNode*> muls;
        std::vector<AddOpNode*> ops;

        std::string toString();
    };

    class CmpOpNode{
    public:
        yolexer::yoToken* op = nullptr;

        std::string toString();
    };

    class BoolOpNode{
    public:
        yolexer::yoToken* op = nullptr;

        std::string toString();
    };

    class CmpExprNode{
    public:
        CmpOpNode* reverse = nullptr;
        AddExprNode* expr = nullptr;
        CmpOpNode* op = nullptr;
        AddExprNode* cmpExpr = nullptr;

        std::string toString();
    };

    class BoolExprNode{
    public:
        std::vector<CmpExprNode*> cmps;
        std::vector<BoolOpNode*> ops;

        std::string toString();
    };

    class StructExprNode{
    public:
        yolexer::yoToken* left;
        std::vector<yolexer::yoToken*> dots;
        std::vector<WholeExprNode*> elements;
        yolexer::yoToken* right;

        std::string toString();
    };

    class NewExprNode {
    public:
        yolexer::yoToken* mark = nullptr;
        IdentifierExprNode* iden = nullptr;
        StructExprNode* initlist = nullptr;

        std::string toString();
    };

    class AssignmentExprNode{
    public:
        IdentifierExprNode* iden = nullptr;
        yolexer::yoToken* equ = nullptr;
        WholeExprNode* expr = nullptr;

        std::string toString();
    };

    class TypecastExprNode{
    public:
        IdentifierExprNode* expr = nullptr;
        AsOpNode* op = nullptr;
        yolexer::yoToken* type = nullptr;

        std::string toString();
    };

    class WholeExprNode{
    public:
        AddExprNode* addexpr = nullptr;
        BoolExprNode* boolexpr = nullptr;
        ListExprNode* listexpr = nullptr;
        StructExprNode* strexpr = nullptr;
        NewExprNode* newexpr = nullptr;
        AssignmentExprNode* assign = nullptr;

        std::string toString();
    };

    class StfOpNode{ // 类似于函数的运算符
    public:
        yolexer::yoToken* name = nullptr;
        yolexer::yoToken* left = nullptr;
        WholeExprNode* expr = nullptr;
        yolexer::yoToken* right = nullptr;

        std::string toString();
    };

    class ListExprNode{
    public:
        yolexer::yoToken* left = nullptr;
        std::vector<WholeExprNode*> elements;
        std::vector<yolexer::yoToken*> dots;
        yolexer::yoToken* right = nullptr;

        std::string toString();
    };

    // STMT
    class OutStmtNode{
    public:
        yolexer::yoToken* mark = nullptr; // mark为语句的标识
        WholeExprNode* expr = nullptr;
        yolexer::yoToken* end = nullptr; // end为语句末尾符，也就是;

        std::string toString();
    };

    class StructDefineStmtNode{
    public:
        struct memberPair {
            yolexer::yoToken* type = nullptr;
            yolexer::yoToken* sep = nullptr;
            yolexer::yoToken* name = nullptr;
            std::string toString();
        };

        yolexer::yoToken* mark = nullptr;
        yolexer::yoToken* name = nullptr;
        yolexer::yoToken* left = nullptr;
        std::vector<memberPair*> members;
        std::vector<yolexer::yoToken*> dots;
        yolexer::yoToken* right = nullptr;

        std::string toString();
    };

    class LetStmtNode{
    public:
        struct defineBlock {
            yolexer::yoToken* name = nullptr;
            yolexer::yoToken* separate = nullptr;
            yolexer::yoToken* type = nullptr;
            yolexer::yoToken* equ = nullptr;
            WholeExprNode* expr = nullptr;

            std::string toString();
        };

        yolexer::yoToken* modifier = nullptr;
        yolexer::yoToken* mark = nullptr;    // only let

        std::vector<defineBlock*> defintions;
        std::vector<yolexer::yoToken*> dots;

        yolexer::yoToken* end = nullptr;

        std::string toString();
    };

    class SpExprStmtNode{
    public:
        SiadExprNode* siad = nullptr;
        AssignmentExprNode* assign = nullptr;
        IdentifierExprNode* iden = nullptr;
        TypecastExprNode* typecast = nullptr;
        yolexer::yoToken* end = nullptr;

        std::string toString();
    };

    class BlockStmtNode {
    public:
        yolexer::yoToken* left = nullptr;
        std::vector<StmtNode*> stmts;
        yolexer::yoToken* right = nullptr;

        std::string toString();
    };

    class PackDefineStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        BlockStmtNode* body = nullptr;

        std::string toString();
    };

    class IfStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        yolexer::yoToken* left = nullptr;
        BoolExprNode* cond = nullptr;
        yolexer::yoToken* right = nullptr;
        BlockStmtNode* body = nullptr;

        std::string toString();
    };

    class ElifStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        yolexer::yoToken* left = nullptr;
        BoolExprNode* cond = nullptr;
        yolexer::yoToken* right = nullptr;
        BlockStmtNode* body = nullptr;

        std::string toString();
    };

    class ElseStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        BlockStmtNode* body = nullptr;

        std::string toString();
    };

    class DeleteStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        IdentifierExprNode* iden = nullptr;
        yolexer::yoToken* end = nullptr;

        std::string toString();
    };

    class WhileStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        yolexer::yoToken* left = nullptr;
        BoolExprNode* cond = nullptr;
        yolexer::yoToken* right = nullptr;
        BlockStmtNode* body = nullptr;

        std::string toString();
    };

    class RepeatStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        yolexer::yoToken* left = nullptr;
        AddExprNode* times = nullptr;
        yolexer::yoToken* right = nullptr;
        BlockStmtNode* body = nullptr;

        std::string toString();
    };

    class ForStmtNode {
    public:
        bool hasVorc = false; // 是否有for括号内的第一项（变量/常量声明
        bool hasCond = false; // 是否有for括号内的第二项（条件判断
        bool hasOp = false; // 是否有for括号内的第三项（操作

        yolexer::yoToken* mark = nullptr;
        yolexer::yoToken* left = nullptr;
        LetStmtNode* vorc = nullptr;
        yolexer::yoToken* sep1 = nullptr;
        BoolExprNode* cond = nullptr;
        yolexer::yoToken* sep2 = nullptr;
        AssignmentExprNode* assign = nullptr;
        SiadExprNode* siad = nullptr;
        yolexer::yoToken* right = nullptr;

        BlockStmtNode* body = nullptr;

        std::string toString();
    };

    class FuncDefStmtNode {
    public:
        bool hasPara = false; // 判断一个函数有没有参数

        struct para {
            yolexer::yoToken* paraname = nullptr;
            yolexer::yoToken* parasep = nullptr;
            yolexer::yoToken* paratype = nullptr;
        };

        yolexer::yoToken* mark = nullptr;
        yolexer::yoToken* rettype = nullptr;
        yolexer::yoToken* name = nullptr;
        yolexer::yoToken* left = nullptr;
        std::vector<para*> paras;
        std::vector<yolexer::yoToken*> dots;
        yolexer::yoToken* right = nullptr;
        BlockStmtNode* body = nullptr;

        std::string toString();
    };

    class ReturnStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        WholeExprNode* expr = nullptr;
        yolexer::yoToken* end = nullptr;

        std::string toString();
    };

    class BreakStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        yolexer::yoToken* end = nullptr;

        std::string toString();
    };

    class DeferStmtNode {
    public:
        yolexer::yoToken* mark = nullptr;
        StmtNode* stmt = nullptr;

        std::string toString();
    };

    class StmtNode{
    public:
        OutStmtNode* outstmt = nullptr;
        BreakStmtNode* breakstmt = nullptr;
        LetStmtNode* vorcstmt = nullptr;
        BlockStmtNode* blockstmt = nullptr;
        SpExprStmtNode* spexprstmt = nullptr;
        IfStmtNode* ifstmt = nullptr;
        ElifStmtNode* elifstmt = nullptr;
        ElseStmtNode* elsestmt = nullptr;
        WhileStmtNode* whilestmt = nullptr;
        RepeatStmtNode* repeatstmt = nullptr;
        DeleteStmtNode* delstmt = nullptr;
        ForStmtNode* forstmt = nullptr;
        FuncDefStmtNode* fdefstmt = nullptr;
        DeferStmtNode* deferstmt = nullptr;
        ReturnStmtNode* retstmt = nullptr;
        StructDefineStmtNode* sdefstmt = nullptr;

        std::string toString();
    };
}