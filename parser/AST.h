#pragma once
#include "../lexer/lexer.h"

namespace AST{
    class AddExprNode; class FuncCallNode; class ListExprNode; class WholeExprNode;

    class IdentifierNode{
    public:
        std::vector<yolexer::yoToken*> idens;
        std::vector<yolexer::yoToken*> dots;

        std::string toString();
    };

    class SiadExprNode{
    public:
        bool isFront = false; // 是否为前置
        IdentifierNode* iden = nullptr;
        yolexer::yoToken* op = nullptr;

        std::string toString();
    };

    class IndexOpNode{
    public:
        yolexer::yoToken* left = nullptr;
        AddExprNode* index = nullptr;
        yolexer::yoToken* right = nullptr;

        std::string toString();
    };

    class PrimExprNode{
    public:
        yolexer::yoToken* number = nullptr;
        yolexer::yoToken* string = nullptr;
        yolexer::yoToken* null = nullptr;
        yolexer::yoToken* character = nullptr;
        IdentifierNode* iden = nullptr;
        FuncCallNode* fcall = nullptr;
        SiadExprNode* siad = nullptr;

        // 括号
        yolexer::yoToken* left = nullptr;
        WholeExprNode* expr = nullptr;
        yolexer::yoToken* right = nullptr;

        IndexOpNode* op = nullptr;

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
        yolexer::yoToken* reverse = nullptr;
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

    class FuncCallNode{
        bool isBif = false;
    public:
        IdentifierNode* iden = nullptr;
        yolexer::yoToken* left = nullptr;
        std::vector<WholeExprNode*> paras;
        std::vector<yolexer::yoToken*> dots;
        yolexer::yoToken* right = nullptr;
        
        std::string toString();
    };

    class WholeExprNode{
    public:
        AddExprNode* addexpr = nullptr;
        BoolExprNode* boolexpr = nullptr;
        ListExprNode* listexpr = nullptr;

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
}