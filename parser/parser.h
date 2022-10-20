#include "AST.h"

namespace parser{
    // stmt的符号
    enum stmtSym {
        outStmt, spStmt, vorcStmt, blockStmt, whileStmt, ifStmt, elifStmt, elseStmt, forStmt, repeatStmt, deleteStmt, breakStmt, funcdefStmt,
        returnStmt, structdefStmt, packdefStmt, wholeStmt
    };
    // wholeStmt做标识，代表所有的stmt

    class Parser{
        std::vector<yolexer::yoToken> tg;
        int offset = 0; //偏移量，用来确定当前到哪个token了
    public:
        Parser(std::vector<yolexer::yoToken> _tg);
        yolexer::yoToken* peek(int _offset = 0); // 获取当前tokenGroup的token，不偏移
        yolexer::yoToken* token(); // 获取昂前tokenGroup的token，然后偏移

        // 判断区: isXXX ---> 判断是什么Node
        bool isPrim(); bool isIdentifierExpr(); bool isSiadExpr(); bool isMulExpr(); bool isStructExpr(); bool isIdentifier();
        bool isAddExpr(); bool isCmpExpr(); bool isBoolExpr(); bool isCellExpr(); bool isNewExpr();
        bool isAddOp(); bool isMulOp(); bool isCmpOp(); bool isBoolOp(); bool isIndexOp(); bool isStfOp(); bool isCallOp();
        bool isAsOp(); bool isListExpr(); bool isAssignmentExpr(); bool isTypecastExpr(); bool isCellIdentifier(); bool isExpr();

        bool isStmt(); bool isOutStmt(); bool isVorcStmt(); bool isSpExprStmt(); bool isBlockStmt();
        bool isWhileStmt(); bool isIfStmt(); bool isElifStmt(); bool isElseStmt(); bool isForStmt();
        bool isRepeatStmt(); bool isDeleteStmt(); bool isBreakStmt(); bool isFuncDefStmt(); bool isDeferStmt();
        bool isReturnStmt(); bool isStructDefStmt(); bool isPackDefStmt();
        // 生成区: parseXXX ---> 生成XXXNode
        AST::SiadExprNode* parseSiadExprNode();
        AST::CellExprNode* parseCellExprNode();
        AST::CallOpNode* parseCallOpNode();
        AST::CellIdentifierNode* parseCellIdentifierNode();
        AST::IdentifierNode* parseIdentifierNode();
        AST::IdentifierExprNode* parseIdentifierExprNode();
        AST::IndexOpNode* parseIndexOpNode();
        AST::AddOpNode* parseAddOpNode();
        AST::MulOpNode* parseMulOpNode();
        AST::CmpOpNode* parseCmpOpNode();
        AST::AsOpNode* parseAsOp();
        AST::BoolOpNode* parseBoolOpNode();
        AST::NewExprNode* parseNewExprNode();
        AST::StfOpNode* parseStfOpNode();
        AST::ListExprNode* parseListExprNode();
        AST::PrimExprNode* parsePrimExprNode();
        AST::MulExprNode* parseMulExprNode();
        AST::AddExprNode* parseAddExprNode();
        AST::CmpExprNode* parseCmpExprNode();
        AST::BoolExprNode* parseBoolExprNode();
        AST::StructExprNode* parseStructExprNode();
        AST::TypecastExprNode* parseTypecastExprNode();
        AST::AssignmentExprNode* parseAssignmentExprNode();

        AST::OutStmtNode* parseOutStmtNode();
        AST::VorcStmtNode* parseVorcStmtNode(bool asStmt=true);
        AST::PackDefineStmtNode* parsePackDefineStmtNode();
        AST::SpExprStmtNode* parseSpExprStmtNode();
        AST::BlockStmtNode* parseBlockStmtNode();
        AST::WhileStmtNode* parseWhileStmtNode();
        AST::IfStmtNode* parseIfStmtNode();
        AST::ElifStmtNode* parseElifStmtNode();
        AST::ElseStmtNode* parseElseStmtNode();
        AST::RepeatStmtNode* parseRepeatStmtNode();
        AST::DeleteStmtNode* parseDeleteStmtNode();
        AST::ForStmtNode* parseForStmtNode();
        AST::BreakStmtNode* parseBreakStmtNode();
        AST::FuncDefStmtNode* parseFuncDefStmtNode();
        AST::StructDefineStmtNode* parseStructDefStmtNode();
        AST::DeferStmtNode* parseDeferStmtNode();
        AST::ReturnStmtNode* parseReturnStmtNode();
        AST::StmtNode* parseStmtNode();

        // 特例：不是以自己的Node命名的函数，因为它有特殊的用途：用于解析所有Expression
        AST::WholeExprNode* parseExpr(); 
        // 特里：不是以自己的Node命名的函数，因为它有特殊的用途，用于解析所有Statement
        std::vector<AST::StmtNode*> parse();

    };
}