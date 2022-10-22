#include "parser.h"

// FUNCTIONALITY
parser::Parser::Parser(std::vector<yolexer::yoToken> _tg): tg(_tg), offset(0) {}
yolexer::yoToken* parser::Parser::peek(int _offset){
    return new yolexer::yoToken(tg[static_cast<unsigned long long int>(offset + _offset)]);
}

yolexer::yoToken* parser::Parser::token(){
    return new yolexer::yoToken(tg[static_cast<unsigned long long int>(offset++)]);
}

// CHECKER

bool parser::Parser::isPrim() {
    return peek()->type == yolexer::yoTokType::String ||
            peek()->type == yolexer::yoTokType::Integer || peek()->type == yolexer::yoTokType::Decimal ||
            peek()->type == yolexer::yoTokType::Identifier || isStfOp() || 
            peek()->content == "null" || peek()->content == "true" || peek()->content == "false" ||
            peek()->content == "++" || peek()->content == "--" || peek()->content == "(" || isTypecastExpr() ||
            isCellExpr();
}
bool parser::Parser::isIdentifier() {
    if(isCellIdentifier()){
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
bool parser::Parser::isIdentifierExpr() {
    return isIdentifier();
}
bool parser::Parser::isSiadExpr() {
    if(peek()->type == yolexer::yoTokType::Identifier){
        int temp = offset; // 存档记位
        parseIdentifierExprNode(); // 生成identifier，看后面是不是++或者--
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
bool parser::Parser::isStructExpr() {
    return peek()->content == "{";
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
bool parser::Parser::isCellExpr() {
    if(isAddOp() || peek()->content == "&" || peek()->content == "@" || peek()->content == "%" || peek()->content == "*") {
        int temp = offset;
        token();
        if(isPrim()){
            offset = temp;
            return true;
        }
        else{
            offset = temp;
            return false;
        }
    }
    else return false;
}
bool parser::Parser::isNewExpr() {
    return peek()->content == "new";
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
bool parser::Parser::isAsOp() {
    return peek()->content == "as";
}
bool parser::Parser::isTypecastExpr() {
    if(isIdentifier()) {
        int temp = offset;
        parseIdentifierExprNode();
        if(peek()->content == "as") {
            offset = temp;
            return true;
        }
        else {
            offset = temp;
            return false;
        }
    }
    else return false;
}
bool parser::Parser::isCallOp() {
    return peek()->content == "(";
}
bool parser::Parser::isCellIdentifier() {
    return peek()->type == yolexer::yoTokType::Identifier;
}
bool parser::Parser::isAssignmentExpr() {
    if(isIdentifier()){
        int temp = offset; // 存档记位
        parseIdentifierExprNode();
        if(isIndexOp()){
            parseIndexOpNode();
            if(peek()->content == "="||peek()->content == "+="||peek()->content == "-=") {
                offset = temp; // 归位
                return true;
            }
            else {
                offset = temp; // 归位
                return false;
            }
        }
        else {
            if(peek()->content == "="||peek()->content == "+="||peek()->content == "-=") {
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
    return isListExpr() || isAddExpr() || isBoolExpr() || isAssignmentExpr() || isNewExpr() || isStructExpr();
}
bool parser::Parser::isOutStmt() {
    return peek()->content == "out";
}
bool parser::Parser::isVorcStmt() {
    return peek()->content == "var" || peek()->content == "const" || peek()->content == "dynamic" || peek()->content == "static";
}
bool parser::Parser::isBlockStmt() {
    return peek()->content == "{";
}
bool parser::Parser::isWhileStmt() {
    return peek()->content == "while";
}
bool parser::Parser::isSpExprStmt() {
    return isSiadExpr() || isAssignmentExpr() || isIdentifierExpr() || isTypecastExpr();
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
    return peek()->content == "func";
}
bool parser::Parser::isDeferStmt() {
    return peek()->content == "defer";
}
bool parser::Parser::isReturnStmt() {
    return peek()->content == "return";
}
bool parser::Parser::isStructDefStmt() {
    return peek()->content == "struct";
}
bool parser::Parser::isPackDefStmt() {
    return peek()->content == "pack";
}
bool parser::Parser::isStmt() {
    return isOutStmt() || isVorcStmt() || isSpExprStmt() || isBlockStmt() || isWhileStmt() || isIfStmt() || isElifStmt() || isElseStmt() ||
            isRepeatStmt() || isDeleteStmt() || isForStmt() || isBreakStmt() || isFuncDefStmt() || isDeferStmt() || isReturnStmt() || isStructDefStmt() ||
            isPackDefStmt();
}

// EXPR

AST::CellExprNode* parser::Parser::parseCellExprNode(){
    AST::CellExprNode* node = new AST::CellExprNode;
    node->op = token();
    if(isPrim()) node->expr = parsePrimExprNode();
    else throw yoexception::YoError("SyntaxError", "Expect a primary expression", tg[offset].line, tg[offset].column);
    return node;
}
AST::SiadExprNode* parser::Parser::parseSiadExprNode(){
    AST::SiadExprNode* node = new AST::SiadExprNode; // 所有的ASTparser第一行都应该这么写： AST::XXXXNode* node = new AST::XXXXNode;
    if(peek()->content == "--" || peek()->content == "++" || peek()->type == yolexer::yoTokType::Identifier) {
        if(peek()->content == "--" || peek()->content == "++") {
            node->isFront = true; // 是前置
            node->op = token();
            node->iden = parseIdentifierExprNode();
        }
        else if(peek()->type == yolexer::yoTokType::Identifier) {
            node->iden = parseIdentifierExprNode();
            node->op = token();
        }
    }
    else throw yoexception::YoError("SyntaxError", "Unexpected content: '" + tg[offset].content + "'", 
                tg[offset].line, 
                tg[offset].column);
    return node;
}
AST::CallOpNode* parser::Parser::parseCallOpNode() {
    AST::CallOpNode* node = new AST::CallOpNode;
    node->left = token();
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
AST::CellIdentifierNode* parser::Parser::parseCellIdentifierNode() {
    AST::CellIdentifierNode* node = new AST::CellIdentifierNode;
    node->iden = token();
    if(isCallOp()) node->call = parseCallOpNode();
    return node;
}
AST::IdentifierNode* parser::Parser::parseIdentifierNode() {
    AST::IdentifierNode* node = new AST::IdentifierNode;
    node->iden = parseCellIdentifierNode();
    if(isIndexOp()) node->idx = parseIndexOpNode();
    return node;
}
AST::IdentifierExprNode* parser::Parser::parseIdentifierExprNode(){
    AST::IdentifierExprNode* node = new AST::IdentifierExprNode;
    node->idens.push_back(parseIdentifierNode());
    while(true){
        if(peek()->content != ".") break;
        node->dots.push_back(token());
        node->idens.push_back(parseIdentifierNode());
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
AST::AsOpNode* parser::Parser::parseAsOp() {
    AST::AsOpNode* node = new AST::AsOpNode;
    node->op = token();
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
AST::NewExprNode* parser::Parser::parseNewExprNode() {
    AST::NewExprNode* node = new AST::NewExprNode;
    node->mark = token();
    if(isIdentifier()) node->iden = parseIdentifierExprNode();
    else throw yoexception::YoError("SyntaxError", "Expect an identifier!", tg[offset].line, tg[offset].column);
    if(isStructExpr()) node->initlist = parseStructExprNode();
    else throw yoexception::YoError("SyntaxError", "Expect an initialization list!", tg[offset].line, tg[offset].column);
    return node;

}
AST::PrimExprNode* parser::Parser::parsePrimExprNode(){
    AST::PrimExprNode* node = new AST::PrimExprNode;
    if(isTypecastExpr()) node->typecast = parseTypecastExprNode();
    else if(peek()->type == yolexer::yoTokType::Integer ||
            peek()->type == yolexer::yoTokType::Decimal) node->number = token();
    else if(peek()->type == yolexer::yoTokType::String) node->string = token();
    else if(peek()->content == "null") node->null = token();
    else if(peek()->content == "true" || peek()->content == "false") node->boolconst = token();
    else if(isStfOp()) node->stf = parseStfOpNode();
    else if(isCellExpr()) node->cellexpr = parseCellExprNode();
    else if(isIdentifierExpr()) {
        node->iden = parseIdentifierExprNode();
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
    else throw yoexception::YoError("SyntaxError", "Unsupported expression or operator", tg[offset].line, tg[offset].column);
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
AST::StructExprNode* parser::Parser::parseStructExprNode() {
    AST::StructExprNode* node = new AST::StructExprNode;
    node->left = token();

    if(isExpr()) {
        node->elements.push_back(parseExpr());
        while(true) {
            if(peek()->content != ",") break;
            node->dots.push_back(token());

            if(isExpr()) node->elements.push_back(parseExpr());
            else throw yoexception::YoError("SyntaxError", "Expect an expression!", tg[offset].line, tg[offset].column);
        }
    }
    else ;

    if(peek()->content == "}") node->right = token();
    else throw  yoexception::YoError("SyntaxError", "Expect '}'!", tg[offset].line, tg[offset].column);
    return node;
}
AST::TypecastExprNode* parser::Parser::parseTypecastExprNode() {
    AST::TypecastExprNode* node = new AST::TypecastExprNode;
    node->expr = parseIdentifierExprNode();
    node->op = parseAsOp();
    if(std::find(yolexer::typeList.begin(),
                 yolexer::typeList.end(),
                 peek()->content) != yolexer::typeList.end()) {
        node->type = token();
    }
    else throw yoexception::YoError("SyntaxError", "Requires a type specifier", tg[offset].line, tg[offset].column);
    return node;
}
AST::AssignmentExprNode* parser::Parser::parseAssignmentExprNode() {
    AST::AssignmentExprNode* node = new AST::AssignmentExprNode;
    node->iden = parseIdentifierExprNode();
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
    else if(isStructExpr()) node->strexpr = parseStructExprNode();
    else if(isNewExpr()) node->newexpr = parseNewExprNode();
    return node;
}

// STMT

AST::StmtNode* parser::Parser::parseStmtNode() {
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
    else if(isStructDefStmt()) node->sdefstmt = parseStructDefStmtNode();
    else if(isBreakStmt()) node->breakstmt = parseBreakStmtNode();
    else if(isFuncDefStmt()) node->fdefstmt = parseFuncDefStmtNode();
    else if(isDeferStmt()) node->deferstmt = parseDeferStmtNode();
    else if(isReturnStmt()) node->retstmt = parseReturnStmtNode();
    else if(isSpExprStmt()) node->spexprstmt = parseSpExprStmtNode();
    else throw yoexception::YoError("SyntaxError", "Not any statement", tg[offset].line, tg[offset].column);
    return node;
}

std::vector<AST::StmtNode*> parser::Parser::parse(){
    std::vector<AST::StmtNode*> stmts;
    while(isStmt()){
        AST::StmtNode* node = new AST::StmtNode;
        node = parseStmtNode();
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

AST::PackDefineStmtNode* parser::Parser::parsePackDefineStmtNode() {
    AST::PackDefineStmtNode* node = new AST::PackDefineStmtNode;
    node->mark = token();

}

AST::VorcStmtNode* parser::Parser::parseVorcStmtNode(bool asStmt){
    AST::VorcStmtNode* node = new AST::VorcStmtNode;
    if(peek()->content == "dynamic" || peek()->content == "static") {
        node->modifier = token();
        if(peek()->content == "const")
            throw yoexception::YoError("SyntaxError", "Constants cannot use modifier: '" + node->modifier->content + "'", tg[offset].line, tg[offset].column);
        node->mark = token();
    }
    else node->mark = token();

    AST::VorcStmtNode::defineBlock* defintion = new AST::VorcStmtNode::defineBlock;
    if(isIdentifier()) defintion->name = token();
    else throw yoexception::YoError("SyntaxError", "Expect an identifier", tg[offset].line, tg[offset].column);
    if(peek()->content == ":"){
        defintion->separate = token();
        if(peek()->type == yolexer::KeyWord) defintion->type = token();
        else throw yoexception::YoError("SyntaxError", "Expect type specifier ", tg[offset].line, tg[offset].column);
        if(peek()->content == "=") {
            defintion->equ = token();
            if(isExpr()) {
                if(isNewExpr() && defintion->type != nullptr)
                    throw yoexception::YoError("SyntaxError", "Cannot use new expression to initialize a non primitive type when specifying a primitive type", tg[offset].line, tg[offset].column);
                defintion->expr = parseExpr();
            }
            else throw yoexception::YoError("SyntaxError", "Expect an expression", tg[offset].line, tg[offset].column);
        }
    }
    else{
        if(peek()->content == "=") {
            defintion->equ = token();
            if (isExpr()) defintion->expr = parseExpr();
            else throw yoexception::YoError("SyntaxError", "Expect an expression", tg[offset].line, tg[offset].column);
        }
    }

    node->defintions.push_back(defintion);
    while(true) {
        if(peek()->content != ",") break;
        node->dots.push_back(token());

        AST::VorcStmtNode::defineBlock* def = new AST::VorcStmtNode::defineBlock;
        if(isIdentifier()) def->name = token();
        else throw yoexception::YoError("SyntaxError", "Expect an identifier", tg[offset].line, tg[offset].column);
        if(peek()->content == ":"){
            def->separate = token();
            if(peek()->type == yolexer::KeyWord) def->type = token();
            else throw yoexception::YoError("SyntaxError", "Expect type specifier ", tg[offset].line, tg[offset].column);
            if(peek()->content == "=") {
                def->equ = token();
                if(isExpr()) {
                    if(isNewExpr() && defintion->type != nullptr)
                        throw yoexception::YoError("SyntaxError", "Cannot use new expression to initialize a non primitive type when specifying a primitive type", tg[offset].line, tg[offset].column);
                    defintion->expr = parseExpr();
                }
                else throw yoexception::YoError("SyntaxError", "Expect an expression", tg[offset].line, tg[offset].column);
            }
        }
        else{
            if(peek()->content == "=") {
                def->equ = token();
                if (isExpr()) def->expr = parseExpr();
                else throw yoexception::YoError("SyntaxError", "Expect an expression", tg[offset].line, tg[offset].column);
            }
        }

        node->defintions.push_back(def);
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
    else if(isIdentifierExpr()) {
        node->iden = parseIdentifierExprNode();
        if(node->iden->idens[node->iden->idens.size() - 1]->iden->call == nullptr)
            throw yoexception::YoError("SyntaxError", "Not any statement", tg[offset].line, tg[offset].column);
    }
    else if(isTypecastExpr()) node->typecast = parseTypecastExprNode();
    else throw yoexception::YoError("SyntaxError", "Not any statement", tg[offset].line, tg[offset].column);
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
    if(isIdentifier()) node->iden = parseIdentifierExprNode();
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

AST::DeferStmtNode* parser::Parser::parseDeferStmtNode() {
    AST::DeferStmtNode* node = new AST::DeferStmtNode;
    node->mark = token();
    if(isStmt()) node->stmt = parseStmtNode();
    else throw yoexception::YoError("SyntaxError", "Not any statement", tg[offset].line, tg[offset].column);
    return node;
}

AST::ReturnStmtNode* parser::Parser::parseReturnStmtNode() {
    AST::ReturnStmtNode* node = new AST::ReturnStmtNode;
    node->mark = token();
    if(isExpr()) node->expr = parseExpr();
    else throw yoexception::YoError("SyntaxError", "Expect an expression!", tg[offset].line, tg[offset].column);

    if(peek()->content == ";") node->end = token();
    else throw yoexception::YoError("SyntaxError", "Expect ';'", tg[offset].line, tg[offset].column);

    return node;
}

AST::StructDefineStmtNode* parser::Parser::parseStructDefStmtNode() {
    AST::StructDefineStmtNode* node = new AST::StructDefineStmtNode;
    node->mark = token();
    if(peek()->type == yolexer::yoTokType::Identifier) node->name = token();
    else throw  yoexception::YoError("SyntaxError", "Expect an identifier!", tg[offset].line, tg[offset].column);
    if(peek()->content == "{") node->left = token();
    else throw  yoexception::YoError("SyntaxError", "Expect '{'!", tg[offset].line, tg[offset].column);

    if(peek()->type != yolexer::yoTokType::Identifier)
        throw yoexception::YoError("SyntaxError", "A structure cannot have no members", tg[offset].line, tg[offset].column);
    AST::StructDefineStmtNode::memberPair* temp = new AST::StructDefineStmtNode::memberPair;
    temp->name = token();
    if(peek()->content == ":") temp->sep = token();
    else throw  yoexception::YoError("SyntaxError", "Expect ':'!", tg[offset].line, tg[offset].column);
    temp->type = token();
    node->members.push_back(temp);
    while(true) {
        if(peek()->content != ",") break;
        node->dots.push_back(token());

        temp = new AST::StructDefineStmtNode::memberPair;
        temp->name = token();
        if(peek()->content == ":") temp->sep = token();
        else throw  yoexception::YoError("SyntaxError", "Expect ':'!", tg[offset].line, tg[offset].column);
        temp->type = token();
        node->members.push_back(temp);
    }

    if(peek()->content == "}") node->right = token();
    else throw  yoexception::YoError("SyntaxError", "Expect '}'!", tg[offset].line, tg[offset].column);
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
