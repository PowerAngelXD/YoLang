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
            peek()->type == yolexer::yoTokType::Identifier || isStfOp() || 
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
        if(peek()->content == "true" || peek()->content == "false") return true;
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
bool parser::Parser::isStfOp() {
    return peek()->type == yolexer::yoTokType::KeyWord && peek(1)->content == "(";
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
    return peek()->content == "var" || peek()->content == "const"; // TODO： 支持ref后添加对ref的判断
}
bool parser::Parser::isBlockStmt() {
    return peek()->content == "{";
}
bool parser::Parser::isWhileStmt() {
    return peek()->content == "while";
}
bool parser::Parser::isSpExprStmt() {
    return isSiadExpr() || isAssignmentExpr() || isFnCallExpr();
}
bool parser::Parser::isIfStmt() {
    return peek()->content == "if";
}
bool parser::Parser::isElifStmt() {
    return peek()->content == "elif";
}
bool parser::Parser::isElseStmt() {
    return peek()->content == "else";
}
bool parser::Parser::isForStmt() {
    return peek()->content == "for";
}
bool parser::Parser::isRepeatStmt() {
    return peek()->content == "repeat";
}
bool parser::Parser::isDeleteStmt() {
    return peek()->content == "delete";
}
bool parser::Parser::isBreakStmt() {
    return peek()->content == "break";
}
bool parser::Parser::isFuncDefStmt() {
    return peek()->content == "function";
}
bool parser::Parser::isStmt() {
    return isOutStmt() || isVorcStmt() || isSpExprStmt() || isBlockStmt() || isWhileStmt() || isIfStmt() || isElifStmt() || isElseStmt() ||
            isRepeatStmt() || isDeleteStmt() || isForStmt() || isBreakStmt() || isFuncDefStmt();
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
    else throw yoexception::YoError("SyntaxError", "A correct expression is required",
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
AST::StfOpNode* parser::Parser::parseStfOpNode(){
    AST::StfOpNode* node = new AST::StfOpNode;
    node->name = token();
    if(peek()->content == "(") node->left = token();
    else throw yoexception::YoError("SyntaxError", "Expect '('", tg[offset].line,tg[offset].column);
    if(isExpr()) node->expr = parseExpr();
    if(peek()->content == ")") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect ')'", tg[offset].line,tg[offset].column);
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
    else if(isStfOp()) node->stf = parseStfOpNode();
    else if(isIdentifier()) {
        node->iden = parseIdentifierNode();
        if(isIndexOp()) node->op = parseIndexOpNode();
    }
    else if(isSiadExpr()) node->siad = parseSiadExprNode();
    else if(peek()->content == "("){
        node->left = token();
        node->expr = parseExpr();
        if(peek()->content == ")") node->right = token();
        else throw yoexception::YoError("SyntaxError", "Expect ')'",
                tg[offset].line,
                tg[offset].column);
    }
    return node;
}
AST::FuncCallNode* parser::Parser::parseFuncCallNode(){
    AST::FuncCallNode* node = new AST::FuncCallNode;
    node->iden = parseIdentifierNode();
    if(peek()->content == "(") node->left = token();
    else throw yoexception::YoError("SyntaxError", "Expect '('",
                                    tg[offset].line,
                                    tg[offset].column);
    if(isExpr()) {
        node->paras.push_back(parseExpr());
        while(peek()->content == ",") {
            node->dots.push_back(token());
            node->paras.push_back(parseExpr());
        }
    }
    if(peek()->content == ")") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect ')'",
                                    tg[offset].line,
                                    tg[offset].column);
    return node;
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
    else throw yoexception::YoError("SyntaxError", "Expect ']'",
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

// STMT

std::vector<AST::StmtNode*> parser::Parser::parse(){
    std::vector<AST::StmtNode*> stmts;
    while(isStmt()){
        AST::StmtNode* node = new AST::StmtNode;
        if(isOutStmt()) node->outstmt = parseOutStmtNode();
        else if(isBlockStmt()) node->blockstmt = parseBlockStmtNode();
        else if(isVorcStmt()) node->vorcstmt = parseVorcStmtNode();
        else if(isWhileStmt()) node->whilestmt = parseWhileStmtNode();
        else if(isIfStmt()) node->ifstmt = parseIfStmtNode();
        else if(isElifStmt()) node->elifstmt = parseElifStmtNode();
        else if(isElseStmt()) node->elsestmt = parseElseStmtNode();
        else if(isRepeatStmt()) node->repeatstmt = parseRepeatStmtNode();
        else if(isDeleteStmt()) node->delstmt = parseDeleteStmtNode();
        else if(isForStmt()) node->forstmt = parseForStmtNode();
        else if(isSpExprStmt()) node->spexprstmt = parseSpExprStmtNode();
        else if(isBreakStmt()) node->breakstmt = parseBreakStmtNode();
        else if(isFuncDefStmt()) node->fdefstmt = parseFuncDefStmtNode();
        else throw yoexception::YoError("SyntaxError", "Not any statement", tg[offset].line, tg[offset].column);
        stmts.push_back(node);
    }
    if(stmts.empty())
        throw yoexception::YoError("SyntaxError", "Not any statement", tg[offset].line, tg[offset].column);
    return stmts;}

AST::OutStmtNode* parser::Parser::parseOutStmtNode(){
    AST::OutStmtNode* node = new AST::OutStmtNode;
    node->mark = token();
    if(isExpr()) node->expr = parseExpr();
    else throw yoexception::YoError("SyntaxError", "An expression is required here", tg[offset].line, tg[offset].column);
    if(peek()->content == ";") node->end = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    return node;
}

AST::VorcStmtNode* parser::Parser::parseVorcStmtNode(bool asStmt){
    AST::VorcStmtNode* node = new AST::VorcStmtNode;
    node->mark = token();
    if(isIdentifier()) node->name = token();
    else throw yoexception::YoError("SyntaxError", "Expect an identifier", tg[offset].line, tg[offset].column);
    if(peek()->content == ":"){
        node->separate = token();
        if(peek()->type == yolexer::KeyWord) node->type = token();
        else throw yoexception::YoError("SyntaxError", "Expect type specifier ", tg[offset].line, tg[offset].column);
        if(peek()->content == "=") {
            node->equ = token();
            if(isExpr()) node->expr = parseExpr();
            else throw yoexception::YoError("SyntaxError", "Expect an expression", tg[offset].line, tg[offset].column);
        }
    }
    else{
        if(peek()->content == "=") {
            node->equ = token();
            if (isExpr()) node->expr = parseExpr();
            else throw yoexception::YoError("SyntaxError", "Expect an expression", tg[offset].line, tg[offset].column);
        }
    }
    if(peek()->content == ";" && asStmt) node->end = token();
    else {
        if(asStmt)
            throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    }
    return node;
}

AST::SpExprStmtNode* parser::Parser::parseSpExprStmtNode() {
    AST::SpExprStmtNode* node = new AST::SpExprStmtNode;
    if(isSiadExpr()) node->siad = parseSiadExprNode();
    else if(isAssignmentExpr()) node->assign = parseAssignmentExprNode();
    else if(isFnCallExpr()) node->fcall = parseFuncCallNode();
    if(peek()->content == ";") node->end = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    return node;
}

AST::BlockStmtNode* parser::Parser::parseBlockStmtNode() {
    AST::BlockStmtNode* node = new AST::BlockStmtNode;
    node->left = token();
    if(isStmt()) node->stmts = parse();
    if(peek()->content == "}") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect '}'", tg[offset].line, tg[offset].column);
    return node;
}

AST::WhileStmtNode* parser::Parser::parseWhileStmtNode() {
    AST::WhileStmtNode* node = new AST::WhileStmtNode;
    node->mark = token();
    if(peek()->content == "(") node->left = token();
    else throw yoexception::YoError("SyntaxError", "Expect '('", tg[offset].line, tg[offset].column);
    if(isBoolExpr() || peek()->content == "true" || peek()->content == "false") node->cond = parseBoolExprNode();
    else throw yoexception::YoError("SyntaxError", "Expect a boolean expression", tg[offset].line, tg[offset].column);
    if(peek()->content == ")") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect ')'", tg[offset].line, tg[offset].column);
    if(isBlockStmt()) node->body = parseBlockStmtNode();
    else throw yoexception::YoError("SyntaxError", "Expect a block statement", tg[offset].line, tg[offset].column);
    return node;
}

AST::IfStmtNode* parser::Parser::parseIfStmtNode() {
    AST::IfStmtNode* node = new AST::IfStmtNode;
    node->mark = token();
    if(peek()->content == "(") node->left = token();
    else throw yoexception::YoError("SyntaxError", "Expect '('", tg[offset].line, tg[offset].column);
    if(isBoolExpr() || peek()->content == "true" || peek()->content == "false") node->cond = parseBoolExprNode();
    else throw yoexception::YoError("SyntaxError", "Expect a boolean expression", tg[offset].line, tg[offset].column);
    if(peek()->content == ")") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect ')'", tg[offset].line, tg[offset].column);
    if(isBlockStmt()) node->body = parseBlockStmtNode();
    else throw yoexception::YoError("SyntaxError", "Expect a block statement", tg[offset].line, tg[offset].column);
    return node;
}

AST::ElifStmtNode* parser::Parser::parseElifStmtNode() {
    AST::ElifStmtNode* node = new AST::ElifStmtNode;
    node->mark = token();
    if(peek()->content == "(") node->left = token();
    else throw yoexception::YoError("SyntaxError", "Expect '('", tg[offset].line, tg[offset].column);
    if(isBoolExpr() || peek()->content == "true" || peek()->content == "false") node->cond = parseBoolExprNode();
    else throw yoexception::YoError("SyntaxError", "Expect a boolean expression", tg[offset].line, tg[offset].column);
    if(peek()->content == ")") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect ')'", tg[offset].line, tg[offset].column);
    if(isBlockStmt()) node->body = parseBlockStmtNode();
    else throw yoexception::YoError("SyntaxError", "Expect a block statement", tg[offset].line, tg[offset].column);
    return node;
}

AST::ElseStmtNode* parser::Parser::parseElseStmtNode() {
    AST::ElseStmtNode* node = new AST::ElseStmtNode;
    node->mark = token();
    if(isBlockStmt()) node->body = parseBlockStmtNode();
    else throw yoexception::YoError("SyntaxError", "Expect a block statement", tg[offset].line, tg[offset].column);
    return node;
}

AST::ForStmtNode* parser::Parser::parseForStmtNode() {
    AST::ForStmtNode* node = new AST::ForStmtNode;
    node->mark = token();
    if(peek()->content == "(") node->left = token();
    else throw yoexception::YoError("SyntaxError", "Expect '('", tg[offset].line, tg[offset].column);
    if(isVorcStmt()) {
        node->vorc = parseVorcStmtNode(false);
        node->hasVorc = true;
    }
    if(peek()->content == ";") node->sep1 = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    if(isBoolExpr()) {
        node->cond = parseBoolExprNode();
        node->hasCond = true;
    }
    if(peek()->content == ";") node->sep2 = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    if(isAssignmentExpr()) {
        node->assign = parseAssignmentExprNode();
        node->hasOp = true;
    }
    else if(isSiadExpr()) {
        node->siad = parseSiadExprNode();
        node->hasOp = true;
    }
    if(peek()->content == ")") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect ')'", tg[offset].line, tg[offset].column);
    if(isBlockStmt()) node->body = parseBlockStmtNode();
    else throw yoexception::YoError("SyntaxError", "Expect a block statement", tg[offset].line, tg[offset].column);
    return node;
}

AST::RepeatStmtNode* parser::Parser::parseRepeatStmtNode() {
    AST::RepeatStmtNode* node = new AST::RepeatStmtNode;
    node->mark = token();
    if(peek()->content == "(") node->left = token();
    else throw yoexception::YoError("SyntaxError", "Expect '('", tg[offset].line, tg[offset].column);
    if(isAddExpr()) node->times = parseAddExprNode();
    else throw yoexception::YoError("SyntaxError", "Expect an addExpression!", tg[offset].line, tg[offset].column);
    if(peek()->content == ")") node->right = token();
    else throw yoexception::YoError("SyntaxError", "Expect ')'", tg[offset].line, tg[offset].column);
    if(isBlockStmt()) node->body = parseBlockStmtNode();
    else throw yoexception::YoError("SyntaxError", "Expect a block statement", tg[offset].line, tg[offset].column);
    return node;
}

AST::DeleteStmtNode* parser::Parser::parseDeleteStmtNode() {
    AST::DeleteStmtNode* node = new AST::DeleteStmtNode;
    node->mark = token();
    if(isIdentifier()) node->iden = parseIdentifierNode();
    else throw yoexception::YoError("SyntaxError", "Expect an identifier!", tg[offset].line, tg[offset].column);
    if(peek()->content == ";") node->end = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    return node;
}

AST::BreakStmtNode* parser::Parser::parseBreakStmtNode() {
    AST::BreakStmtNode* node = new AST::BreakStmtNode;
    node->mark = token();
    if(peek()->content == ";") node->end = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);
    return node;
}

AST::FuncDefStmtNode* parser::Parser::parseFuncDefStmtNode() {
    AST::FuncDefStmtNode* node = new AST::FuncDefStmtNode;
    node->mark = token();
    if(std::find(yolexer::typeList.begin(), yolexer::typeList.end(), peek()->content) != yolexer::typeList.end()) {
        // 是typename，继续生成
        node->rettype = token();
        if(peek()->type == yolexer::yoTokType::Identifier) {
            node->name = token();
            if(peek()->content == "(") {
                node->left = token();
                if(peek()->type == yolexer::yoTokType::Identifier) {
                    node->hasPara = true;
                    AST::FuncDefStmtNode::para* temp = new AST::FuncDefStmtNode::para;
                    temp->paraname = token();
                    temp->parasep = token();
                    temp->paratype = token();
                    node->paras.push_back(temp);
                    while(true) {
                        if(peek()->content != ",") break;
                        node->dots.push_back(token());

                        temp = new AST::FuncDefStmtNode::para;
                        temp->paraname = token();
                        temp->parasep = token();
                        temp->paratype = token();
                        node->paras.push_back(temp);
                    }
                }
                else {
                    node->hasPara = false;
                }
                if(peek()->content == ")") {
                    node->right = token();
                    if(isBlockStmt()) {
                        node->body = parseBlockStmtNode();
                        return node;
                    }
                    else throw yoexception::YoError("SyntaxError", "Expect a blockstatement", tg[offset].line, tg[offset].column);
                }
                else throw yoexception::YoError("SyntaxError", "Expect ')'", tg[offset].line, tg[offset].column);
            }
        }
        else throw yoexception::YoError("SyntaxError", "It is not an identifier", tg[offset].line, tg[offset].column);
    }
    else throw yoexception::YoError("SyntaxError", "It is not a valid type name", tg[offset].line, tg[offset].column);
}
