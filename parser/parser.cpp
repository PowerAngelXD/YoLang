#include "parser.h"

// FUNCTIONALITY
parser::Parser::Parser(std::vector<yolexer::yoToken> _tg): tg(_tg), offset(0) {}
yolexer::yoToken* parser::Parser::peek(int _offset){
    return new yolexer::yoToken(tg[offset + _offset]);
}

yolexer::yoToken* parser::Parser::token(){
    return new yolexer::yoToken(tg[offset++]);
}

// CHECKER

bool parser::Parser::isPrim() {
    return peek()->type == yolexer::yoTokType::Character || peek()->type == yolexer::yoTokType::String ||
            peek()->type == yolexer::yoTokType::Integer || peek()->type == yolexer::yoTokType::Decimal ||
            peek()->type == yolexer::yoTokType::Identifier || 
            peek()->content == "null" || peek()->content == "true" || peek()->content == "false" ||
            peek()->content == "++" || peek()->content == "--" || peek()->content == "(";
}
bool parser::Parser::isIdentifier() {
    if(peek()->type == yolexer::yoTokType::Identifier){
        int temp = offset; // 存档记位
        parseIdentifierNode(); // 生成identifier，看后面是不是++或者--
        if(peek()->content == "++" || peek()->content == "--") {
            offset = temp; // 归位
            return false;
        }
        else {
            offset = temp; // 归位
            return true;
        }
    }
    else return false;
}
bool parser::Parser::isSiadExpr() {
    if(peek()->type == yolexer::yoTokType::Identifier){
        int temp = offset; // 存档记位
        parseIdentifierNode(); // 生成identifier，看后面是不是++或者--
        if(peek()->content == "++" || peek()->content == "--") {
            offset = temp; // 归位
            return true;
        }
        else {
            offset = temp; // 归位
            return false;
        }
    }
    else if(peek()->content == "++" || peek()->content == "--"){
        int temp = offset; // 存档记位
        token(); // 生成token，看后面是不是Identifier
        if(peek()->type == yolexer::yoTokType::Identifier) {
            offset = temp; // 归位
            return true;
        }
        else {
            offset = temp; // 归位
            return false;
        }
    }
    else return false;
}
bool parser::Parser::isMulExpr() {
    return isPrim();
}
bool parser::Parser::isAddExpr() {
    return isMulExpr();
} 
bool parser::Parser::isCmpExpr() {
    if(isAddExpr()){
        int temp = offset; // 存档记位
        parseAddExprNode(); // 生成AddExpr，看后面是不是CmpOp
        if(isCmpOp()) {
            offset = temp; // 归位
            return true;
        }
        else if(isBoolOp()) {
            offset = temp;
            return true;
        }
        else {
            offset = temp; // 归位
            return false;
        }
    }
    else if(peek()->content == "!") return true;
    else return false;
}
bool parser::Parser::isBoolExpr() {
    return isCmpExpr();
}
bool parser::Parser::isFnCallExpr() {
    if(isIdentifier()){
        int temp = offset; // 存档记位
        parseIdentifierNode(); // 生成identifier，看后面是不是括号
        if(peek()->content == "(") {
            offset = temp; // 归位
            return true;
        }
        else {
            offset = temp; // 归位
            return false;
        }
    }
    else return false;
}
bool parser::Parser::isAddOp() {
    return peek()->content == "+" || peek()->content == "-";
}
bool parser::Parser::isMulOp() {
    return peek()->content == "/" || peek()->content == "*" || peek()->content == "%";
}
bool parser::Parser::isCmpOp() {
    return peek()->content == "==" || peek()->content == "!=" || peek()->content == ">=" || peek()->content == "<="
            || peek()->content == "<" || peek()->content == ">" || peek()->content == "!";
}
bool parser::Parser::isBoolOp() {
    return peek()->content == "||" || peek()->content == "&&";
}
bool parser::Parser::isIndexOp() {
    return peek()->content == "[";
}
bool parser::Parser::isListExpr() {
    return peek()->content == "[";
}
bool parser::Parser::isAssignmentExpr() {
    if(isIdentifier()){
        int temp = offset; // 存档记位
        parseIdentifierNode(); // 生成identifier，看后面是不是括号
        if(isIndexOp()){
            parseIndexOpNode();
            if(peek()->content == "=") {
                offset = temp; // 归位
                return true;
            }
            else {
                offset = temp; // 归位
                return false;
            }
        }
        else {
            if(peek()->content == "=") {
                offset = temp; // 归位
                return true;
            }
            else {
                offset = temp; // 归位
                return false;
            }
        }
    }
    else return false;
}
bool parser::Parser::isExpr() {
    return isListExpr() || isAddExpr() || isBoolExpr();
}
bool parser::Parser::isOutStmt() {
    return peek()->content == "out";
}
bool parser::Parser::isVorcStmt() {
    return peek()->content == "var" || peek()->content == "const";
}
bool parser::Parser::isStmt() {
    return isOutStmt() || isVorcStmt();
}

// EXPR

AST::SiadExprNode* parser::Parser::parseSiadExprNode(){
    AST::SiadExprNode* node = new AST::SiadExprNode; // 所有的ASTparser第一行都应该这么写： AST::XXXXNode* node = new AST::XXXXNode;
    if(peek()->content == "--" || peek()->content == "++" || peek()->type == yolexer::yoTokType::Identifier) {
        if(peek()->content == "--" || peek()->content == "++") {
            node->isFront = true; // 是前置
            node->op = token();
            node->iden = parseIdentifierNode();
        }
        else if(peek()->type == yolexer::yoTokType::Identifier) {
            node->iden = parseIdentifierNode();
            node->op = token();
        }
    }
    else throw yoexception::YoError("SyntaxError", "Unexpected content: '" + tg[offset].content + "'", 
                tg[offset].line, 
                tg[offset].column);
    return node;
}
AST::IdentifierNode* parser::Parser::parseIdentifierNode(){
    AST::IdentifierNode* node = new AST::IdentifierNode;
    node->idens.push_back(token());
    while(true){
        if(peek()->content != ".") break;
        node->dots.push_back(token());
        node->idens.push_back(token());
    }
    return node;
}
AST::IndexOpNode* parser::Parser::parseIndexOpNode(){
    AST::IndexOpNode* node = new AST::IndexOpNode;
    node->left = token();
    if(isAddExpr()) node->index = parseAddExprNode();
    else throw yoexception::YoError("SynaxError", "A correct expression is required", 
                tg[offset].line,
                tg[offset].column);
    if(peek()->content == "]") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect ']'", 
                tg[offset].line,
                tg[offset].column);
    return node;
}
AST::AddOpNode* parser::Parser::parseAddOpNode(){
    if(!isAddOp()) throw yoexception::YoError("SyntaxError", "Expect '+' or '-'", 
                        tg[offset].line,
                        tg[offset].column);
    AST::AddOpNode* node = new AST::AddOpNode;
    node->op = token();
    return node;
}
AST::MulOpNode* parser::Parser::parseMulOpNode(){
    if(!isMulOp()) throw yoexception::YoError("SyntaxError", "Expect '*', '%' or '/'", 
                        tg[offset].line,
                        tg[offset].column);
    AST::MulOpNode* node = new AST::MulOpNode;
    node->op = token();
    return node;
}
AST::CmpOpNode* parser::Parser::parseCmpOpNode(){
    if(!isCmpOp()) throw yoexception::YoError("SyntaxError", "Expect '==', '!=', '>', '>=', '<' or '<='", 
                        tg[offset].line,
                        tg[offset].column);
    AST::CmpOpNode* node = new AST::CmpOpNode;
    node->op = token();
    return node;
}
AST::BoolOpNode* parser::Parser::parseBoolOpNode(){
    if(!isBoolOp()) throw yoexception::YoError("SyntaxError", "Expect '||' or '&&'", 
                        tg[offset].line,
                        tg[offset].column);
    AST::BoolOpNode* node = new AST::BoolOpNode;
    node->op = token();
    return node;
}
AST::PrimExprNode* parser::Parser::parsePrimExprNode(){
    AST::PrimExprNode* node = new AST::PrimExprNode;
    if(isFnCallExpr()) node->fcall = parseFuncCallNode();
    else if(peek()->type == yolexer::yoTokType::Integer ||
            peek()->type == yolexer::yoTokType::Decimal) node->number = token();
    else if(peek()->type == yolexer::yoTokType::Character) node->character = token();
    else if(peek()->type == yolexer::yoTokType::String) node->string = token();
    else if(peek()->content == "null") node->null = token();
    else if(peek()->content == "true" || peek()->content == "false") node->boolconst = token();
    else if(isIdentifier()) {
        node->iden = parseIdentifierNode();
        if(isIndexOp()) node->op = parseIndexOpNode();
    }
    else if(isSiadExpr()) node->siad = parseSiadExprNode();
    else if(peek()->content == "("){
        node->left = token();
        node->expr = parseExpr();
        if(peek()->content == ")") node->right = token();
        else throw yoexception::YoError("SynaxError", "Expect ')'", 
                tg[offset].line,
                tg[offset].column);
    }
    return node;
}
AST::FuncCallNode* parser::Parser::parseFuncCallNode(){
    return nullptr; // TODO: 实现它
}
AST::ListExprNode* parser::Parser::parseListExprNode(){
    AST::ListExprNode* node = new AST::ListExprNode;
    node->left = token();
    if(isExpr()) node->elements.push_back(parseExpr());
    else; // 允许空列表，不报错
    while(true){
        if(peek()->content != ",") break;
        node->dots.push_back(token());
        node->elements.push_back(parseExpr());
    }
    if(peek()->content == "]") node->right = token();
    else throw yoexception::YoError("SynaxError", "Expect ']'", 
                tg[offset].line,
                tg[offset].column);
    return node;
}
AST::MulExprNode* parser::Parser::parseMulExprNode(){
    AST::MulExprNode* node = new AST::MulExprNode;
    if(isPrim()) node->prims.push_back(parsePrimExprNode());
    else throw yoexception::YoError("SyntaxError", "Expect a primary expression!", tg[offset].line, tg[offset].column);
    while(true){
        if(!isMulOp()) break;
        node->ops.push_back(parseMulOpNode());
        node->prims.push_back(parsePrimExprNode());
    }
    return node;
}
AST::AddExprNode* parser::Parser::parseAddExprNode(){
    AST::AddExprNode* node = new AST::AddExprNode;
    if(isMulExpr()) node->muls.push_back(parseMulExprNode());
    else throw yoexception::YoError("SyntaxError", "Expect a mul expression!", tg[offset].line, tg[offset].column);
    while(true){
        if(!isAddOp()) break;
        node->ops.push_back(parseAddOpNode());
        node->muls.push_back(parseMulExprNode());
    }
    return node;
}
AST::CmpExprNode* parser::Parser::parseCmpExprNode(){
    AST::CmpExprNode* node = new AST::CmpExprNode;
    if(peek()->content == "!") {
        node->reverse = parseCmpOpNode();
        if(isAddExpr()) node->expr = parseAddExprNode();
        else throw yoexception::YoError("SyntaxError", "An appropriate expression is required!", 
                tg[offset].line,
                tg[offset].column);
    }
    else node->expr = parseAddExprNode();
    if(isCmpOp()) {
        node->op = parseCmpOpNode();
        if(isAddExpr()) node->cmpExpr = parseAddExprNode();
        else throw yoexception::YoError("SyntaxError", "An appropriate expression is required!", 
                tg[offset].line,
                tg[offset].column);
        return node;
    }
    if(isAddExpr()) 
        throw yoexception::YoError("SyntaxError", "Expect '>', '<', '<=', '>=', '==' or '!='!", 
                tg[offset].line,
                tg[offset].column);
    return node;
}
AST::BoolExprNode* parser::Parser::parseBoolExprNode(){
    AST::BoolExprNode* node = new AST::BoolExprNode;
    if(isCmpExpr()) node->cmps.push_back(parseCmpExprNode());
    else throw yoexception::YoError("SyntaxError", "Expect a compare expression!", tg[offset].line, tg[offset].column);
    while(true){
        if(!isBoolOp()) break;
        node->ops.push_back(parseBoolOpNode());
        node->cmps.push_back(parseCmpExprNode());
    }
    return node;
    return node;
}
AST::AssignmentExprNode* parser::Parser::parseAssignmentExprNode() {
    AST::AssignmentExprNode* node = new AST::AssignmentExprNode;
    node->iden = parseIdentifierNode();
    if(isIndexOp()) node->idx = parseIndexOpNode();
    node->equ = token();
    if(isExpr()) node->expr = parseExpr();
    else throw yoexception::YoError("SyntaxError", "An expression is required here", tg[offset].line, tg[offset].column);
    return node;
}
AST::WholeExprNode* parser::Parser::parseExpr(){
    AST::WholeExprNode* node = new AST::WholeExprNode;
    if(isAssignmentExpr()) node->assign = parseAssignmentExprNode();
    else if(isBoolExpr()) node->boolexpr = parseBoolExprNode();
    else if(isAddExpr()) node->addexpr = parseAddExprNode();
    else if(isListExpr()) node->listexpr = parseListExprNode();
    return node;
}

// STNT

std::vector<AST::StmtNode*> parser::Parser::parse(){
    std::vector<AST::StmtNode*> stmts;
    while(isStmt()){
        AST::StmtNode* node = new AST::StmtNode;
        if(isOutStmt()) node->outstmt = parseOutStmtNode();
        else if(isVorcStmt()) node->vorcstmt = parseVorcStmtNode();
        else throw yoexception::YoError("SyntaxError", "Not any statement", tg[offset].line, tg[offset].column);
        stmts.push_back(node);
    }
    if(stmts.empty())
        throw yoexception::YoError("SyntaxError", "Not any statement", tg[offset].line, tg[offset].column);
    return stmts;
}

AST::OutStmtNode* parser::Parser::parseOutStmtNode(){
    AST::OutStmtNode* node = new AST::OutStmtNode;
    node->mark = token();
    if(isExpr()) node->expr = parseExpr();
    else throw yoexception::YoError("SyntaxError", "An expression is required here", tg[offset].line, tg[offset].column);
    if(peek()->content == ";") node->end = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    return node;
}

AST::VorcStmtNode* parser::Parser::parseVorcStmtNode(){
    AST::VorcStmtNode* node = new AST::VorcStmtNode;
    node->mark = token();
    if(isIdentifier()) node->name = token();
    else throw yoexception::YoError("SyntaxError", "Expect an identifier", tg[offset].line, tg[offset].column);
    if(peek()->content == ":"){
        node->separate = token();
        if(peek()->type == yolexer::KeyWord) node->type = token();
        else throw yoexception::YoError("SyntaxError", "Expect type specifier ", tg[offset].line, tg[offset].column);
        if(peek()->content == "=") node->equ = token();
        else throw yoexception::YoError("SyntaxError", "Expect '='", tg[offset].line, tg[offset].column);
        if(isExpr()) node->expr = parseExpr();
        else throw yoexception::YoError("SyntaxError", "Expect an expression", tg[offset].line, tg[offset].column);
    }
    else{
        if(peek()->content == "=") node->equ = token();
        else throw yoexception::YoError("SyntaxError", "Expect '='", tg[offset].line, tg[offset].column);
        if(isExpr()) node->expr = parseExpr();
        else throw yoexception::YoError("SyntaxError", "Expect an expression", tg[offset].line, tg[offset].column);
    }
    if(peek()->content == ";") node->end = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    return node;
}