#include "generator.h"
#define SCOPE_BEGIN minCtor(btc::scopestart, node->left->line, node->left->column);
#define SCOPE_END minCtor(btc::scopeend, node->left->line, node->left->column);
#define PUSH(value, type, line, column) normalCtor(btc::push, value, type, line, column);
#define JMP(cond, mode, line, column) normalCtor(btc::jmp, cond, mode, line, column);
#define SELF_ADD(isfront, line, column) normalCtor(btc::selfadd, isfront, 0, line, column);
#define SELF_SUB(isfront) normalCtor(btc::selfsub, isfront, 0, node->op->line, node->op->column);
#define ADD(line, column) normalCtor(btc::add, 0, 0, line, column);
#define SUB(line, column) normalCtor(btc::sub, 0, 0, line, column);
#define DIV(line, column) normalCtor(btc::div, 0, 0, line, column);
#define MUL(line, column) normalCtor(btc::mul, 0, 0, line, column);
#define TMO normalCtor(btc::tmo, 0, 0, node->op->line, node->op->column);
#define IDX normalCtor(btc::idx, 0, 0, node->left->line, node->left->column);
#define GMEM normalCtor(btc::gmem, 0, 0, node->dots[i]->line, node->dots[i]->column);
#define LST minCtor(btc::lst, node->right->line, node->right->column);
#define GT(line, column) normalCtor(btc::gt, 0, 0, line, column);
#define GTET(line, column) normalCtor(btc::gtet, 0, 0, line, column);
#define LT(line, column) normalCtor(btc::lt, 0, 0, line, column);
#define LTET(line, column) normalCtor(btc::ltet, 0, 0, line, column);
#define EQU(line, column) normalCtor(btc::equ, 0, 0, line, column);
#define NOEQU(line, column) normalCtor(btc::noequ, 0, 0, line, column);
#define LOGIC_AND normalCtor(btc::logicand, 0, 0, node->op->line, node->op->column);
#define LOGIC_OR normalCtor(btc::logicor, 0, 0, node->op->line, node->op->column);
#define NO normalCtor(btc::no, 0, 0, node->op->line, node->op->column);
#define STF(sname) normalCtor(btc::stf, sname, 0, node->name->line, node->name->column);
#define OUT normalCtor(btc::out, 0, 0, node->mark->line, node->mark->column);
#define CREATE(name, type, vtype, vsign) completeCtor(btc::create, name, type, vtype, vsign, node->mark->line, node->mark->column);
#define ASSIGN(isidx) normalCtor(btc::assign, isidx, 0.0, node->equ->line, node->equ->column);
#define DEL(name, type) normalCtor(btc::del, name, type, node->mark->line, node->mark->column);
#define LISTEND minCtor(btc::listend, node->right->line, node->right->column);
#define PARAEND minCtor(btc::paraend, node->right->line, node->left->column);
#define IDENEND(line, column) minCtor(btc::idenend, line, column);
#define CALL minCtor(btc::call, node->left->line, node->left->column);
#define DEL_VAL minCtor(btc::del_val, node->mark->line, node->mark->column);

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
        PUSH(0, ytype::type(ytype::vtype::boolean, ytype::norm), token->line, token->column)
    else if(token->content == "true")
        PUSH(1, ytype::type(ytype::vtype::boolean, ytype::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitNumber(yolexer::yoToken* token){
    if(token->type == yolexer::yoTokType::Integer)
        PUSH(atoi(token->content.c_str()), ytype::type(ytype::vtype::integer, ytype::norm), token->line, token->column)
    else if(token->type == yolexer::yoTokType::Decimal)
        PUSH(atof(token->content.c_str()), ytype::type(ytype::vtype::decimal, ytype::norm), token->line, token->column)
}
void ygen::ByteCodeGenerator::buildIntegerNumber(std::string number, int line, int column) {
    PUSH(atoi(number.c_str()), ytype::type(ytype::vtype::integer, ytype::norm), line, column)
}
void ygen::ByteCodeGenerator::buildDecimalNumber(std::string number, int line, int column) {
    PUSH(atof(number.c_str()), ytype::type(ytype::vtype::decimal, ytype::norm), line, column)
}
void ygen::ByteCodeGenerator::visitString(yolexer::yoToken* token) {
    std::string content = token->content;
    std::string ret;
    for (int i = 0; i < content.size(); i++) {
        if (content[i] == '\\') {
            // 开始判断
            switch (content[++i]) {
                case 'n':
                    ret.push_back('\n');
                    break;
                case '0':
                    ret.push_back('\0');
                    break;
                case 't':
                    ret.push_back('\t');
                    break;
                case 'r':
                    ret.push_back('\r');
                    break;
                default:
                    break;
            }
        } else ret.push_back(content[i]);
    }
    PUSH(addPara(ret), ytype::type(ytype::vtype::string, ytype::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitNull(yolexer::yoToken* token){
    PUSH(0.0, ytype::type(ytype::vtype::null, ytype::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitIdentifier(AST::IdentifierNode* node){
    if(node->dots.empty()){
        PUSH(addPara(node->idens[0]->content), ytype::type(ytype::vtype::iden, ytype::norm), node->idens[0]->line, node->idens[0]->column)
        IDENEND(node->idens[0]->line, node->idens[0]->column)
    }
    else{
        for(int i = 0; i < node->idens.size(); i++) {
            PUSH(addPara(node->idens[i]->content), ytype::type(ytype::vtype::iden, ytype::norm), node->idens[i]->line, node->idens[i]->column);
            if(i == node->idens.size() - 1); // 防止末尾还添加gmem
            else GMEM
        }
        IDENEND(node->idens[0]->line, node->idens[0]->column)
    }
}
void ygen::ByteCodeGenerator::visitIdentifierText(AST::IdentifierNode* node, bool isref){
    std::string text;
    text += node->idens[0]->content;
    for(int i = 0; i < node->dots.size(); i++) {
        text += node->dots[i]->content;
        text += node->idens[i + 1]->content;
    }
    PUSH(addPara(text), ytype::type(ytype::vtype::string, ytype::norm), node->idens[0]->line, node->idens[0]->column);
}
void ygen::ByteCodeGenerator::visitSiadExpr(AST::SiadExprNode* node){
    if(node->isFront){
        // 是前置类型
        if(node->op->content == "++"){
            visitIdentifierText(node->iden);
            SELF_ADD(1.0, node->op->line, node->op->column) // 如果是前置则参数为1，否则为0
        }
        else if(node->op->content == "--"){
            visitIdentifierText(node->iden);
            SELF_SUB(1.0)
        }
    }
    else{
        if(node->op->content == "++"){
            visitIdentifierText(node->iden);
            SELF_ADD(0.0, node->op->line, node->op->column)
        }
        else if(node->op->content == "--"){
            visitIdentifierText(node->iden);
            SELF_SUB(0.0)
        }
    }
}
void ygen::ByteCodeGenerator::visitAddOp(AST::AddOpNode* node){
    if(node->op->content == "+")
        ADD(node->op->line, node->op->column)
    else if(node->op->content == "-") SUB(node->op->line, node->op->column)
}
void ygen::ByteCodeGenerator::visitMulOp(AST::MulOpNode* node){
    if(node->op->content == "*")
        MUL(node->op->line, node->op->column)
    else if(node->op->content == "/")
        DIV(node->op->line, node->op->column)
    else if(node->op->content == "%")
        TMO
}
void ygen::ByteCodeGenerator::visitCmpOp(AST::CmpOpNode* node){
    if(node->op->content == ">")
        GT(node->op->line, node->op->column)
    else if(node->op->content == ">=")
        GTET(node->op->line, node->op->column)
    else if(node->op->content == "<")
        LT(node->op->line, node->op->column)
    else if(node->op->content == "<=")
        LTET(node->op->line, node->op->column)
    else if(node->op->content == "==")
        EQU(node->op->line, node->op->column)
    else if(node->op->content == "!=")
        NOEQU(node->op->line, node->op->column)
    else if(node->op->content == "!")
        NO
}
void ygen::ByteCodeGenerator::visitBoolOp(AST::BoolOpNode* node){
    if(node->op->content == "&&")
        LOGIC_AND
    else if(node->op->content == "||")
        LOGIC_OR
}
void ygen::ByteCodeGenerator::visitIndexOp(AST::IndexOpNode* node){
    visitAddExpr(node->index);
    IDX
}
void ygen::ByteCodeGenerator::visitStfOp(AST::StfOpNode* node){
    if(node->expr != nullptr) visitExpr(node->expr);
    STF(addPara(node->name->content));
}
void ygen::ByteCodeGenerator::visitPrimExpr(AST::PrimExprNode* node){
    if(node->number != nullptr) visitNumber(node->number);
    else if(node->string != nullptr) visitString(node->string);
    else if(node->boolconst != nullptr) visitBoolean(node->boolconst);
    else if(node->null != nullptr) visitNull(node->null);
    else if(node->iden != nullptr) visitIdentifier(node->iden);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
    else if(node->expr != nullptr) visitExpr(node->expr);
    else if(node->stf != nullptr) visitStfOp(node->stf);
    else if(node->fcall != nullptr) visitFuncCallExpr(node->fcall);
    
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
    LISTEND
    for(int i = 0; i < node->elements.size(); i++) {
        visitExpr(node->elements[i]);
    }
    LST
}
void ygen::ByteCodeGenerator::visitAssignmentExpr(AST::AssignmentExprNode* node){
    if (node->idx != nullptr) {
        visitAddExpr(node->idx->index);
        visitIdentifierText(node->iden);
        visitExpr(node->expr);
        ASSIGN(1.0)
    }
    else {
        visitIdentifierText(node->iden);
        visitExpr(node->expr);
        ASSIGN(0.0)
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
void ygen::ByteCodeGenerator::visitFuncCallExpr(AST::FuncCallNode* node) {
    PARAEND
    if(!node->paras.empty()) {
        // 函数调用有参数，生成参数指令
        for(int i = 0; i < node->paras.size(); i ++) {
            visitExpr(node->paras[i]);
        }
        PARAEND
    }
    visitIdentifierText(node->iden); // push一个iden的text用于调用函数
    CALL
}

// STMT

void ygen::ByteCodeGenerator::visitOutStmt(AST::OutStmtNode* node) {
    visitExpr(node->expr);
    OUT
}
void ygen::ByteCodeGenerator::visitVorcStmt(AST::VorcStmtNode* node) {
    if (node->expr == nullptr)
        PUSH(0.0, ytype::type(ytype::vtype::null, ytype::norm), node->mark->line, node->mark->column)
    else
        visitExpr(node->expr);
    CREATE(addPara(node->name->content),
         addPara(node->mark->content),
         node->separate != nullptr ? addPara(node->type->content) : 0.0,
         node->separate != nullptr ? 1.0 : 0.0);
}
void ygen::ByteCodeGenerator::visitSpExprStmt(AST::SpExprStmtNode* node) {
    if(node->assign != nullptr) visitAssignmentExpr(node->assign);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
    else if(node->fcall != nullptr) visitFuncCallExpr(node->fcall);
}
void ygen::ByteCodeGenerator::visitBlockStmt(AST::BlockStmtNode* node) {
    SCOPE_BEGIN
    visit(node->stmts);
    SCOPE_END
}
void ygen::ByteCodeGenerator::visitWhileStmt(AST::WhileStmtNode* node) {
    visitBoolExpr(node->cond);
    JMP(paraHelper::jmpt::reqFalse, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    DEL_VAL
    SCOPE_BEGIN{
        visit(node->body->stmts);
        // 下面的指令用于检测表达式是否还成立，成立则jmp到下一个循环，否则向前偏移
        visitBoolExpr(node->cond);
    }
    SCOPE_END
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::backScope, node->mark->line, node->mark->column)
}
void ygen::ByteCodeGenerator::visitIfStmt(AST::IfStmtNode* node) {
    visitBoolExpr(node->cond);
    JMP(paraHelper::reqFalse, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}
void ygen::ByteCodeGenerator::visitElifStmt(AST::ElifStmtNode* node) {
    visitBoolExpr(node->cond);
    JMP(paraHelper::reqFalse, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}
void ygen::ByteCodeGenerator::visitElseStmt(AST::ElseStmtNode* node) {
    JMP(paraHelper::reqFalse, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}

void ygen::ByteCodeGenerator::visitRepeatStmt(AST::RepeatStmtNode *node) {
    repit ++;
    PUSH(-1, ytype::type(ytype::vtype::integer, ytype::norm), node->mark->line, node->mark->column)
    CREATE(addPara("_rit" + std::to_string(repit) + "_"), addPara("var"), addPara("integer"), 1.0); // 创建迭代器，初始值设置为1

    PUSH(addPara("_rit" + std::to_string(repit) + "_"), ytype::type(ytype::vtype::iden, ytype::norm), node->mark->line, node->mark->column)
    IDENEND(node->mark->line, node->mark->column);
    visitAddExpr(node->times);
    buildIntegerNumber("1", node->left->line, node->left->column);
    SUB(node->left->line, node->left->column)

    LT(node->right->line, node->right->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    DEL_VAL

    SCOPE_BEGIN{
        PUSH(addPara("_rit" + std::to_string(repit) + "_"), ytype::type(ytype::vtype::string, ytype::norm), node->mark->line,
                   node->mark->column);
        SELF_ADD(0.0, node->left->line, node->left->column)
        visit(node->body->stmts);

        PUSH(addPara("_rit" + std::to_string(repit) + "_"), ytype::type(ytype::vtype::iden, ytype::norm), node->mark->line, node->mark->column)
        IDENEND(node->left->line, node->left->column)
        visitAddExpr(node->times);
        buildIntegerNumber("1", node->left->line, node->left->column);
        SUB(node->left->line, node->left->column)
        LT(node->right->line, node->right->column)
    }
    SCOPE_END

    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::backScope, node->mark->line, node->mark->column)
    DEL_VAL
    // repit的处理
    DEL(addPara("_rit" + std::to_string(repit) + "_"), 1.0)
    repit --;
}

void ygen::ByteCodeGenerator::visitForStmt(AST::ForStmtNode* node) {
    if (node->hasVorc)
        visitVorcStmt(node->vorc);

    if (node->hasCond)
        visitBoolExpr(node->cond);
    else
        PUSH(1.0, ytype::type(ytype::vtype::boolean, ytype::norm), node->mark->line, node->mark->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    DEL_VAL
    SCOPE_BEGIN{
        visit(node->body->stmts);
        if (node->assign != nullptr)
            visitAssignmentExpr(node->assign);
        else if (node->siad != nullptr)
            visitSiadExpr(node->siad);

        if (node->hasCond)
            visitBoolExpr(node->cond);
        else
            PUSH(1.0, ytype::type(ytype::vtype::boolean, ytype::norm), node->mark->line, node->mark->column)
    }SCOPE_END
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::backScope, node->mark->line, node->mark->column)
    DEL_VAL
    // 释放局部变量
    if(node->hasVorc)
        DEL(addPara(node->vorc->name->content), 1.0)
}

void ygen::ByteCodeGenerator::visitDeleteStmt(AST::DeleteStmtNode* node) {
    visitIdentifierText(node->iden);
    DEL(0, 0)
}

void ygen::ByteCodeGenerator::visitBreakStmt(AST::BreakStmtNode* node) {
    PUSH(0.0, ytype::type(ytype::vtype::boolean, ytype::norm), node->mark->line, node->mark->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
}

void ygen::ByteCodeGenerator::visitFuncDefStmt(AST::FuncDefStmtNode* node) {
    PUSH(addPara(node->rettype->content), ytype::type(ytype::vtype::string, ytype::norm), node->mark->line, node->mark->column); // 将要定义的函数的返回值类型入栈
    PARAEND // 因为栈的原因，flag应该置后
    if(node->hasPara) {
        for(int i = 0; i < node->paras.size(); i++) {
            PUSH(addPara(node->paras[i]->paratype->content), ytype::type(ytype::vtype::string, ytype::norm), node->paras[i]->paratype->line, node->paras[i]->paratype->column);
            PUSH(addPara(node->paras[i]->paraname->content), ytype::type(ytype::vtype::string, ytype::norm), node->paras[i]->paraname->line, node->paras[i]->paraname->column);
        }
    }
    CREATE(addPara(node->name->content), addPara(node->mark->content), addPara(node->rettype->content), 1.0)
    visitBlockStmt(node->body);
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
        else if(stmt->forstmt != nullptr) visitForStmt(stmt->forstmt);
        else if(stmt->breakstmt != nullptr) visitBreakStmt(stmt->breakstmt);
        else if(stmt->fdefstmt != nullptr) visitFuncDefStmt(stmt->fdefstmt);
    }
}
