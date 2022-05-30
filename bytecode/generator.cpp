#include "generator.h"

ygen::ByteCodeGenerator::ByteCodeGenerator(std::vector<AST::StmtNode*> _stmts): stmts(_stmts) {}
ygen::ByteCodeGenerator::ByteCodeGenerator(AST::WholeExprNode* _expr): expr(_expr) {}
std::string ygen::ByteCodeGenerator::removeZero(float content) {
    std::ostringstream oss;
    oss<<content;
    return oss.str();
}
std::vector<std::string> ygen::ByteCodeGenerator::getConstPool() {
    return constpool;
}
std::vector<ygen::byteCode> ygen::ByteCodeGenerator::getCodes() {
    return codes;
}
int ygen::ByteCodeGenerator::addPara(std::string para){
    constpool.push_back(para);
    return constpool.size()-1;
}
void ygen::ByteCodeGenerator::minCtor(ygen::btc code, int ln, int col){
    codes.push_back({code, 0.0, 0.0, 0.0, 0.0, ln, col});
}
void ygen::ByteCodeGenerator::normalCtor(ygen::btc code, float arg1, float arg2, int ln, int col){
    codes.push_back({code, arg1, arg2, 0.0, 0.0, ln, col});
}
void ygen::ByteCodeGenerator::completeCtor(ygen::btc code, float arg1, float arg2, float arg3, float arg4, int ln, int col){
    codes.push_back({code, arg1, arg2, arg3, arg4, ln, col});
}
void ygen::ByteCodeGenerator::genFile(std::string name) {
    std::ofstream file(name, std::ios::binary|std::ios::out);
    
    std::string paraArea;
    for(int i = 0; i < codes.size(); i++) {
        file<<removeZero(codes[i].code)<<",";
        file<<removeZero(codes[i].arg1)<<",";
        file<<removeZero(codes[i].arg2)<<",";
        file<<removeZero(codes[i].arg3)<<",";
        file<<removeZero(codes[i].arg4)<<",";
        file<<removeZero(codes[i].line)<<",";
        file<<removeZero(codes[i].column)<<":\n";
    }
    file<<"#\n";
    for(auto para: constpool) {
        file<<para<<"\"";
        // file.write(strcat(para.data(), ","), sizeof(std::string));
    }
    file.close();
}

void ygen::ByteCodeGenerator::visitBoolean(yolexer::yoToken* token){
    if(token->content == "false")
        normalCtor(btc::push, 0, paraHelper::boolean, token->line, token->column);
    else if(token->content == "true")
        normalCtor(btc::push, 1, paraHelper::boolean, token->line, token->column);
}
void ygen::ByteCodeGenerator::visitNumber(yolexer::yoToken* token){
    if(token->type == yolexer::yoTokType::Integer)
        normalCtor(btc::push, atoi(token->content.c_str()), paraHelper::integer, token->line, token->column);
    else if(token->type == yolexer::yoTokType::Decimal)
        normalCtor(btc::push, atof(token->content.c_str()), paraHelper::decimal, token->line, token->column);
}
void ygen::ByteCodeGenerator::visitStrCh(yolexer::yoToken* token){
    if(token->type == yolexer::yoTokType::Character){
        if(token->content.size() > 1 && token->content[0] != '\\')
            throw yoexception::YoError("CharError", "This character is too long",
                    token->line, token->column);
        std::string content = token->content;
        std::string ret; // 处理之后的结果
        if(content[0] == '\\'){
            switch (content[1])
            {
            case 'n': ret.push_back('\n'); break;
            case '0': ret.push_back('\0'); break;
            case 't': ret.push_back('\t'); break;
            case 'r': ret.push_back('\r'); break;
            default: break;
            }
        }
        else ret = content[0];
        normalCtor(btc::push, addPara(ret), paraHelper::character, token->line, token->column);
    }
    else if(token->type == yolexer::yoTokType::String){
        std::string content = token->content;
        std::string ret;
        for(int i = 0; i < content.size(); i++){
            if(content[i] == '\\'){
                // 开始判断
                switch (content[++ i])
                {
                case 'n': ret.push_back('\n'); break;
                case '0': ret.push_back('\0'); break;
                case 't': ret.push_back('\t'); break;
                case 'r': ret.push_back('\r'); break;
                default:
                    break;
                }
            }
            else ret.push_back(content[i]);
        }
        normalCtor(btc::push, addPara(ret), paraHelper::string, token->line, token->column);
    }
}
void ygen::ByteCodeGenerator::visitNull(yolexer::yoToken* token){
    normalCtor(btc::push, 0.0, paraHelper::null, token->line, token->column);
}
void ygen::ByteCodeGenerator::visitIdentifier(AST::IdentifierNode* node){
    if(node->dots.empty()){
        normalCtor(btc::push, addPara(node->idens[0]->content), paraHelper::iden, node->idens[0]->line, node->idens[0]->column);
        minCtor(btc::idenend, node->idens[0]->line, node->idens[0]->column);
    }
    else{
        for(int i = 0; i < node->idens.size(); i++) {
            normalCtor(btc::push, addPara(node->idens[i]->content), paraHelper::iden, node->idens[i]->line, node->idens[i]->column);
            if(i == node->idens.size() - 1); // 防止末尾还添加gmem
            else minCtor(btc::gmem, node->dots[i]->line, node->dots[i]->column);
        }
        minCtor(btc::idenend, node->idens[0]->line, node->idens[0]->column);
    }
}
void ygen::ByteCodeGenerator::visitIdentifierText(AST::IdentifierNode* node, bool isref){
    std::string text;
    text += node->idens[0]->content;
    for(int i = 0; i < node->dots.size(); i++) {
        text += node->dots[i]->content;
        text += node->idens[i + 1]->content;
    }
    normalCtor(btc::push, addPara(text), isref == true?paraHelper::ref:paraHelper::string, node->idens[0]->line, node->idens[0]->column);
}
void ygen::ByteCodeGenerator::visitSiadExpr(AST::SiadExprNode* node){
    if(node->isFront){
        // 是前置类型
        if(node->op->content == "++"){
            visitIdentifierText(node->iden);
            normalCtor(btc::selfadd, 1.0, 0.0, node->op->line, node->op->column); // 前置第一个参数为1，否则为0
        }
        else if(node->op->content == "--"){
            visitIdentifierText(node->iden);
            normalCtor(btc::selfsub, 1.0, 0.0, node->op->line, node->op->column);
        }
    }
    else{
        if(node->op->content == "++"){
            visitIdentifierText(node->iden);
            normalCtor(btc::selfadd, 0.0, 0.0, node->op->line, node->op->column);
        }
        else if(node->op->content == "--"){
            visitIdentifierText(node->iden);
            normalCtor(btc::selfsub, 0.0, 0.0, node->op->line, node->op->column);
        }
    }
}
void ygen::ByteCodeGenerator::visitAddOp(AST::AddOpNode* node){
    if(node->op->content == "+")
        minCtor(btc::add, node->op->line, node->op->column);
    else if(node->op->content == "-") minCtor(btc::sub, node->op->line, node->op->column);
}
void ygen::ByteCodeGenerator::visitMulOp(AST::MulOpNode* node){
    if(node->op->content == "*")
        minCtor(btc::mul, node->op->line, node->op->column);
    else if(node->op->content == "/")
        minCtor(btc::div, node->op->line, node->op->column);
    else if(node->op->content == "%")
        minCtor(btc::tmo, node->op->line, node->op->column);
}
void ygen::ByteCodeGenerator::visitCmpOp(AST::CmpOpNode* node){
    if(node->op->content == ">")
        minCtor(btc::gt, node->op->line, node->op->column);
    else if(node->op->content == ">=")
        minCtor(btc::gtet, node->op->line, node->op->column);
    else if(node->op->content == "<")
        minCtor(btc::lt, node->op->line, node->op->column);
    else if(node->op->content == "<=")
        minCtor(btc::ltet, node->op->line, node->op->column);
    else if(node->op->content == "==")
        minCtor(btc::equ, node->op->line, node->op->column);
    else if(node->op->content == "!=")
        minCtor(btc::noequ, node->op->line, node->op->column);
    else if(node->op->content == "!")
        minCtor(btc::no, node->op->line, node->op->column);
}
void ygen::ByteCodeGenerator::visitBoolOp(AST::BoolOpNode* node){
    if(node->op->content == "&&")
        minCtor(btc::logicand, node->op->line, node->op->column);
    else if(node->op->content == "||")
        minCtor(btc::logicor, node->op->line, node->op->column);
}
void ygen::ByteCodeGenerator::visitIndexOp(AST::IndexOpNode* node){
    visitAddExpr(node->index);
    minCtor(btc::idx, node->left->line, node->left->column);
}
void ygen::ByteCodeGenerator::visitStfOp(AST::StfOpNode* node){
    if(node->expr != nullptr) visitExpr(node->expr);
    normalCtor(btc::stf, addPara(node->name->content), 0.0, node->name->line, node->name->column);
}
void ygen::ByteCodeGenerator::visitPrimExpr(AST::PrimExprNode* node){
    if(node->number != nullptr) visitNumber(node->number);
    else if(node->string != nullptr) visitStrCh(node->string);
    else if(node->boolconst != nullptr) visitBoolean(node->boolconst);
    else if(node->character != nullptr) visitStrCh(node->character);
    else if(node->null != nullptr) visitNull(node->null);
    else if(node->iden != nullptr) visitIdentifier(node->iden);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
    else if(node->expr != nullptr) visitExpr(node->expr);
    else if(node->stf != nullptr) visitStfOp(node->stf);
    
    if(node->op != nullptr) visitIndexOp(node->op);
}
void ygen::ByteCodeGenerator::visitMulExpr(AST::MulExprNode* node){
    visitPrimExpr(node->prims[0]);
    for(int i = 0; i < node->ops.size(); i++) {
        visitPrimExpr(node->prims[i + 1]);
        visitMulOp(node->ops[i]);
    }
}
void ygen::ByteCodeGenerator::visitAddExpr(AST::AddExprNode* node){
    visitMulExpr(node->muls[0]);
    for(int i = 0; i < node->ops.size(); i++) {
        visitMulExpr(node->muls[i + 1]);
        visitAddOp(node->ops[i]);
    }
}
void ygen::ByteCodeGenerator::visitCmpExpr(AST::CmpExprNode* node){
    visitAddExpr(node->expr);
    if(node->op != nullptr){
        visitAddExpr(node->cmpExpr);
        visitCmpOp(node->op);
    }
    if(node->reverse != nullptr)
        visitCmpOp(node->reverse);
}
void ygen::ByteCodeGenerator::visitBoolExpr(AST::BoolExprNode* node){
    visitCmpExpr(node->cmps[0]);
    for(int i = 0; i < node->ops.size(); i++) {
        visitCmpExpr(node->cmps[i + 1]);
        visitBoolOp(node->ops[i]);
    }
}
void ygen::ByteCodeGenerator::visitListExpr(AST::ListExprNode* node){
    minCtor(btc::listend, node->right->line, node->right->column);
    for(int i = 0; i < node->elements.size(); i++) {
        visitExpr(node->elements[i]);
    }
    minCtor(btc::lst, node->right->line, node->right->column);
}
void ygen::ByteCodeGenerator::visitAssignmentExpr(AST::AssignmentExprNode* node){
    if (node->idx != nullptr) {
        visitAddExpr(node->idx->index);
        visitIdentifierText(node->iden);
        visitExpr(node->expr);
        normalCtor(btc::assign, 1.0, 0.0, node->equ->line, node->equ->column);
    }
    else {
        visitIdentifierText(node->iden);
        visitExpr(node->expr);
        minCtor(btc::assign, node->equ->line, node->equ->column);
    }
}
void ygen::ByteCodeGenerator::visitExpr(AST::WholeExprNode* node){
    if(node->addexpr != nullptr)
        visitAddExpr(node->addexpr);
    else if(node->boolexpr != nullptr)
        visitBoolExpr(node->boolexpr);
    else if(node->assign != nullptr)
        visitAssignmentExpr(node->assign);
    else if(node->listexpr != nullptr)
        visitListExpr(node->listexpr);
}
void ygen::ByteCodeGenerator::visitFuncCallExpr(AST::FuncCallNode *node) {

}

// STMT

void ygen::ByteCodeGenerator::visitOutStmt(AST::OutStmtNode* node) {
    visitExpr(node->expr);
    minCtor(btc::out, node->mark->line, node->mark->column);
}
void ygen::ByteCodeGenerator::visitVorcStmt(AST::VorcStmtNode* node) {
    if(node->mark->content != "ref") {
        normalCtor(btc::define, addPara(node->name->content), 0.0, node->mark->line, node->mark->column);
        if(node->expr == nullptr)
            normalCtor(btc::push, 0.0, paraHelper::null, node->mark->line, node->mark->column);
        else
            visitExpr(node->expr);
        completeCtor(btc::init, 
                    addPara(node->name->content), 
                    addPara(node->mark->content), 
                    node->separate != nullptr?addPara(node->type->content):0.0, 
                    node->separate != nullptr?1.0:0.0, 
                    node->mark->line, node->mark->column);
    }
    else {
        //  ref定义
        if(node->expr->addexpr == nullptr || node->expr->addexpr->muls[0]->prims[0]->iden == nullptr)
            throw yoexception::YoError("RefError", "Only identifiers can be referenced", 
                                node->mark->line, 
                                node->mark->column);
        normalCtor(btc::define, addPara(node->name->content), 0.0, node->mark->line, node->mark->column);
        visitIdentifierText(node->expr->addexpr->muls[0]->prims[0]->iden, true);
        completeCtor(btc::init, 
                    addPara(node->name->content), 
                    addPara(node->mark->content), 
                    node->separate != nullptr?addPara(node->type->content):0.0, 
                    node->separate != nullptr?1.0:0.0, 
                    node->mark->line, node->mark->column);
    }
}
void ygen::ByteCodeGenerator::visitSpExprStmt(AST::SpExprStmtNode* node) {
    if(node->assign != nullptr) visitAssignmentExpr(node->assign);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
}
void ygen::ByteCodeGenerator::visitBlockStmt(AST::BlockStmtNode* node) {
    minCtor(btc::scopestart, node->left->line, node->left->column);
    visit(node->stmts);
    minCtor(btc::scopeend, node->right->line, node->right->column);
}
void ygen::ByteCodeGenerator::visitWhileStmt(AST::WhileStmtNode* node) {
    visitBoolExpr(node->cond);
    normalCtor(btc::jmp, paraHelper::jmpt::reqTrue, paraHelper::jmpt::outWscope, node->mark->line, node->mark->column); // -1代表如果不满足jmp所需要的条件，则直接跳出scope，如果没有scope则向前偏移
    minCtor(btc::scopestart, node->left->line, node->left->column);
    visit(node->body->stmts);
    // 下面的指令用于检测表达式是否还成立，成立则jmp到下一个循环，否则向前偏移
    visitBoolExpr(node->cond);
    minCtor(btc::scopeend, node->right->line, node->right->column);
    normalCtor(btc::jmp, paraHelper::jmpt::reqTrue, paraHelper::jmpt::findSStart, node->mark->line, node->mark->column);
}
void ygen::ByteCodeGenerator::visitIfStmt(AST::IfStmtNode* node) {
    visitBoolExpr(node->cond);
    normalCtor(btc::jmp, paraHelper::reqTrue, paraHelper::jmpt::outIFscope, node->mark->line, node->mark->column);
    visitBlockStmt(node->body);
}
void ygen::ByteCodeGenerator::visitElifStmt(AST::ElifStmtNode* node) {
    visitBoolExpr(node->cond);
    normalCtor(btc::jmp, paraHelper::reqTrue, paraHelper::jmpt::outElifscope, node->mark->line, node->mark->column);
    visitBlockStmt(node->body);
}
void ygen::ByteCodeGenerator::visitElseStmt(AST::ElseStmtNode* node) {
    normalCtor(btc::jmp, paraHelper::reqTrue, paraHelper::jmpt::outElsescope, node->mark->line, node->mark->column);
    visitBlockStmt(node->body);
}

void ygen::ByteCodeGenerator::visitRepeatStmt(AST::RepeatStmtNode *node) {
    repit ++;
    normalCtor(btc::define, addPara("__repit" + std::to_string(repit) + "__"), 0.0, node->mark->line, node->mark->column);
    normalCtor(btc::push, -1, paraHelper::integer, node->mark->line, node->mark->column);
    completeCtor(btc::init, addPara("__repit" + std::to_string(repit) + "__"),
                                  addPara("var"),
                                  addPara("integer"),
                                  1.0, node->mark->line, node->mark->column); // 创建迭代器，初始值设置为0

    normalCtor(btc::push, addPara("__repit" + std::to_string(repit) + "__"), paraHelper::iden, node->mark->line, node->mark->column);
    minCtor(btc::idenend, node->left->line, node->left->column);
    visitAddExpr(node->times);
    minCtor(btc::lt, node->right->line, node->right->column);
    normalCtor(btc::jmp, paraHelper::jmpt::reqTrue, paraHelper::jmpt::outWscope, node->mark->line, node->mark->column);

    minCtor(btc::scopestart, node->left->line, node->left->column);
    normalCtor(btc::push, addPara("__repit" + std::to_string(repit) + "__"), paraHelper::string, node->mark->line, node->mark->column);
    normalCtor(btc::selfadd, 0.0, 0.0, node->right->line, node->right->column);
    visit(node->body->stmts);

    normalCtor(btc::push, addPara("__repit" + std::to_string(repit) + "__"), paraHelper::iden, node->mark->line, node->mark->column);
    minCtor(btc::idenend, node->left->line, node->left->column);
    visitAddExpr(node->times);
    minCtor(btc::lt, node->right->line, node->right->column);
    minCtor(btc::scopeend, node->left->line, node->left->column);

    normalCtor(btc::jmp, paraHelper::jmpt::reqTrue, paraHelper::jmpt::findSStart, node->mark->line, node->mark->column);
    // repit的处理
    normalCtor(btc::del, addPara("__repit" + std::to_string(repit) + "__"), 1.0, node->mark->line, node->mark->column);
    repit --;
}

void ygen::ByteCodeGenerator::visitDeleteStmt(AST::DeleteStmtNode* node) {
    visitIdentifierText(node->iden);
    normalCtor(btc::del, 0.0, 0.0, node->mark->line, node->mark->column);
}

void ygen::ByteCodeGenerator::visit(std::vector<AST::StmtNode*> stmts) {
    for(auto stmt: stmts){
        if(stmt->outstmt != nullptr) visitOutStmt(stmt->outstmt);
        else if(stmt->blockstmt != nullptr) visitBlockStmt(stmt->blockstmt);
        else if(stmt->vorcstmt != nullptr) visitVorcStmt(stmt->vorcstmt);
        else if(stmt->spexprstmt != nullptr) visitSpExprStmt(stmt->spexprstmt);
        else if(stmt->whilestmt != nullptr) visitWhileStmt(stmt->whilestmt);
        else if(stmt->ifstmt != nullptr) visitIfStmt(stmt->ifstmt);
        else if(stmt->elifstmt != nullptr) visitElifStmt(stmt->elifstmt);
        else if(stmt->elsestmt != nullptr) visitElseStmt(stmt->elsestmt);
        else if(stmt->repeatstmt != nullptr) visitRepeatStmt(stmt->repeatstmt);
        else if(stmt->delstmt != nullptr) visitDeleteStmt(stmt->delstmt);
    }
}
