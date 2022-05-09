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
bool parser::Parser::isExpr() {
    return isListExpr() || isAddExpr() || isBoolExpr();
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
AST::IndexOpNode* parser::Parser::parseIndeOpNode(){
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
    AST::AddOpNode* node = new AST::AddOpNode;
    node->op = token();
    return node;
}
AST::MulOpNode* parser::Parser::parseMulOpNode(){
    AST::MulOpNode* node = new AST::MulOpNode;
    node->op = token();
    return node;
}
AST::CmpOpNode* parser::Parser::parseCmpOpNode(){
    AST::CmpOpNode* node = new AST::CmpOpNode;
    node->op = token();
    return node;
}
AST::BoolOpNode* parser::Parser::parseBoolOpNode(){
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
    else if(isIdentifier()) {
        node->iden = parseIdentifierNode();
        if(isIndexOp()) node->op = parseIndeOpNode();
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
AST::WholeExprNode* parser::Parser::parseExpr(){
    AST::WholeExprNode* node = new AST::WholeExprNode;
    if(isBoolExpr()) node->boolexpr = parseBoolExprNode();
    else if(isAddExpr()) node->addexpr = parseAddExprNode();
    else if(isListExpr()) node->listexpr = parseListExprNode();
    return node;
}