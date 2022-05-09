#include "AST.h"

namespace parser{
    class Parser{
        std::vector<yolexer::yoToken> tg;
        int offset = 0; //偏移量，用来确定当前到哪个token了
    public:
        Parser(std::vector<yolexer::yoToken> _tg);
        yolexer::yoToken* peek(int _offset = 0); // 获取当前tokenGroup的token，不偏移
        yolexer::yoToken* token(); // 获取昂前tokenGroup的token，然后偏移

        // 判断区: isXXX ---> 判断是什么Node
        bool isPrim(); bool isIdentifier(); bool isSiadExpr(); bool isMulExpr();
        bool isAddExpr(); bool isCmpExpr(); bool isBoolExpr(); bool isFnCallExpr();
        bool isAddOp(); bool isMulOp(); bool isCmpOp(); bool isBoolOp(); bool isIndexOp();
        bool isListExpr(); bool isExpr();
        // 生成区: parseXXX ---> 生成XXXNode
        AST::SiadExprNode* parseSiadExprNode();
        AST::IdentifierNode* parseIdentifierNode();
        AST::IndexOpNode* parseIndeOpNode();
        AST::AddOpNode* parseAddOpNode();
        AST::MulOpNode* parseMulOpNode();
        AST::CmpOpNode* parseCmpOpNode();
        AST::BoolOpNode* parseBoolOpNode();
        AST::FuncCallNode* parseFuncCallNode();
        AST::ListExprNode* parseListExprNode();
        AST::PrimExprNode* parsePrimExprNode();
        AST::MulExprNode* parseMulExprNode();
        AST::AddExprNode* parseAddExprNode();
        AST::CmpExprNode* parseCmpExprNode();
        AST::BoolExprNode* parseBoolExprNode();
        AST::WholeExprNode* parseExpr(); // 特例：就这个不是以自己的Node命名的函数，因为它有特殊的用途
    };
}