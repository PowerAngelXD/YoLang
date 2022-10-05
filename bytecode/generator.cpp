#include "generator.h"
#define SCOPE_BEGIN minCtor(btc::scopestart, node->left->line, node->left->column);
#define SCOPE_END minCtor(btc::scopeend, node->left->line, node->left->column);
#define PUSH(value, type, line, column) pushCtor(value, type, 0.0, line, column);
#define JMP(cond, mode, line, column) normalCtor(btc::jmp, cond, mode, line, column);
#define SELF_ADD(isfront, line, column) normalCtor(btc::selfadd, isfront, 0, line, column);
#define SELF_SUB(isfront) normalCtor(btc::selfsub, isfront, 0, node->op->line, node->op->column);
#define ADD(line, column) normalCtor(btc::add, 0, 0, line, column);
#define SUB(line, column) normalCtor(btc::sub, 0, 0, line, column);
#define DIV(line, column) normalCtor(btc::div, 0, 0, line, column);
#define MUL(line, column) normalCtor(btc::mul, 0, 0, line, column);
#define TMO normalCtor(btc::tmo, 0, 0, node->op->line, node->op->column);
#define IDX normalCtor(btc::idx, 0, 0, node->left->line, node->left->column);
#define GMEM normalCtor(btc::gmem, 0, 0, node->dots[0]->line, node->dots[0]->column);
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
#define CREATE(name, type, basicType, vsign) completeCtor(btc::create, name, type, basicType, vsign, node->mark->line, node->mark->column);
#define ASSIGN(isidx) normalCtor(btc::assign, isidx, 0.0, node->equ->line, node->equ->column);
#define DEL(name, type) normalCtor(btc::del, name, type, node->mark->line, node->mark->column);
#define LISTEND minCtor(btc::listend, node->right->line, node->right->column);
#define PARAEND minCtor(btc::paraend, node->right->line, node->left->column);
#define PARAEND_LC(line, column) minCtor(btc::paraend, line, column);
#define TCAST minCtor(btc::tcast, node->op->line, node->op->column);
#define IDENEND(line, column) minCtor(btc::idenend, line, column);
#define CALL minCtor(btc::call, node->left->line, node->left->column);
#define DEL_VAL minCtor(btc::del_val, node->mark->line, node->mark->column);
#define FLAG(type, line, column) normalCtor(btc::flag, type, 0.0, line, column);
#define NEW(line, column) normalCtor(btc::_new, 0, 0, line, column);

ygen::btc ygen::string2Code(std::string s) {
    btc code;
    if(s=="jmp") code=btc::jmp;
    else if(s=="push") code=btc::push;else if(s=="gto") code=btc::gto;else if(s=="tcast") code=btc::tcast;
    else if(s=="cast") code=btc::cast;else if(s=="selfadd") code=btc::selfadd;else if(s=="selfsub") code=btc::selfsub;
    else if(s=="add") code=btc::add;else if(s=="lt") code=btc::lt;else if(s=="scopestart") code=btc::scopestart;
    else if(s=="sub") code=btc::sub;else if(s=="ltet") code=btc::ltet;else if(s=="scopeend") code=btc::scopeend;
    else if(s=="mul") code=btc::mul;else if(s=="gt") code=btc::gt;else if(s=="idenend") code=btc::idenend;
    else if(s=="div") code=btc::div;else if(s=="gtet") code=btc::gtet;else if(s=="flag") code=btc::flag;
    else if(s=="tmo") code=btc::tmo;else if(s=="equ") code=btc::equ;else if(s=="out") code=btc::out;
    else if(s=="idx") code=btc::idx;else if(s=="noequ") code=btc::noequ;else if(s=="create") code=btc::create;
    else if(s=="lst") code=btc::lst;else if(s=="gmem") code=btc::gmem;else if(s=="assign") code=btc::assign;
    else if(s=="logicand") code=btc::logicand;else if(s=="stf") code=btc::stf;else if(s=="del") code=btc::del;
    else if(s=="logicor") code=btc::logicor;else if(s=="listend") code=btc::listend;else if(s=="call") code=btc::call;
    else if(s=="no") code=btc::no;else if(s=="paraend") code=btc::paraend;else if(s=="del_val") code=btc::del_val;
    else if(s=="new") code=btc::_new;
    return code;
}


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
    codes.push_back({code, {ytype::basicType::null, ytype::compType::norm}, 0.0, 0.0, 0.0, 0.0, ln, col});
}
void ygen::ByteCodeGenerator::normalCtor(ygen::btc code, float arg1, float arg2, int ln, int col){
    codes.push_back({code, {ytype::basicType::null, ytype::compType::norm}, arg1, arg2, 0.0, 0.0, ln, col});
}
void ygen::ByteCodeGenerator::completeCtor(ygen::btc code, float arg1, float arg2, float arg3, float arg4, int ln, int col){
    codes.push_back({code, {ytype::basicType::null, ytype::compType::norm}, arg1, arg2, arg3, arg4, ln, col});
}
void ygen::ByteCodeGenerator::pushCtor(float arg1, ytype::ytypeUnit t, float arg2, int ln, int col) {
    codes.push_back({ygen::btc::push, t, arg1, arg2, 0.0, 0.0, ln, col});
}

void ygen::ByteCodeGenerator::visitBoolean(yolexer::yoToken* token){
    if(token->content == "false")
        PUSH(0, ytype::type(ytype::basicType::boolean, ytype::norm), token->line, token->column)
    else if(token->content == "true")
        PUSH(1, ytype::type(ytype::basicType::boolean, ytype::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitNumber(yolexer::yoToken* token){
    if(token->type == yolexer::yoTokType::Integer)
        PUSH(atoi(token->content.c_str()), ytype::type(ytype::basicType::integer, ytype::norm), token->line, token->column)
    else if(token->type == yolexer::yoTokType::Decimal)
        PUSH(atof(token->content.c_str()), ytype::type(ytype::basicType::decimal, ytype::norm), token->line, token->column)
}
void ygen::ByteCodeGenerator::buildIntegerNumber(std::string number, int line, int column) {
    PUSH(atoi(number.c_str()), ytype::type(ytype::basicType::integer, ytype::norm), line, column)
}
void ygen::ByteCodeGenerator::buildDecimalNumber(std::string number, int line, int column) {
    PUSH(atof(number.c_str()), ytype::type(ytype::basicType::decimal, ytype::norm), line, column)
}
void ygen::ByteCodeGenerator::visitString(yolexer::yoToken* token) {
    PUSH(addPara(token->content), ytype::type(ytype::basicType::string, ytype::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitNull(yolexer::yoToken* token){
    PUSH(0.0, ytype::type(ytype::basicType::null, ytype::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitIdentifier(AST::IdentifierNode* node){
    if(node->dots.empty()){
        PUSH(addPara(node->idens[0]->content), ytype::type(ytype::basicType::iden, ytype::norm), node->idens[0]->line, node->idens[0]->column)
    }
    else{
        PUSH(addPara(node->idens[0]->content), ytype::type(ytype::basicType::iden, ytype::norm), node->idens[0]->line, node->idens[0]->column)
        for(int i = 1; i < node->idens.size(); i++) {
            PUSH(addPara(node->idens[i]->content), ytype::type(ytype::basicType::string, ytype::norm), node->idens[i]->line, node->idens[i]->column);
            GMEM
        }
    }
}
void ygen::ByteCodeGenerator::visitIdentifierText(AST::IdentifierNode* node, bool isref){
    std::string text;
    text += node->idens[0]->content;
    for(int i = 0; i < node->dots.size(); i++) {
        text += node->dots[i]->content;
        text += node->idens[i + 1]->content;
    }
    PUSH(addPara(text), ytype::type(ytype::basicType::string, ytype::norm), node->idens[0]->line, node->idens[0]->column);
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
void ygen::ByteCodeGenerator::visitAsOp(AST::AsOpNode *node) {
    TCAST
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
void ygen::ByteCodeGenerator::visitCellExpr(AST::CellExprNode *node) {
    if(node->op->content == "+");
    else if(node->op->content == "-"){
        buildIntegerNumber("0", node->op->line, node->op->column);
        visitPrimExpr(node->expr);
        SUB(node->op->line, node->op->column)
    }
}
void ygen::ByteCodeGenerator::visitPrimExpr(AST::PrimExprNode* node){
    if(node->number != nullptr) visitNumber(node->number);
    else if(node->cellexpr != nullptr) visitCellExpr(node->cellexpr);
    else if(node->string != nullptr) visitString(node->string);
    else if(node->boolconst != nullptr) visitBoolean(node->boolconst);
    else if(node->null != nullptr) visitNull(node->null);
    else if(node->iden != nullptr) visitIdentifier(node->iden);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
    else if(node->expr != nullptr) visitExpr(node->expr);
    else if(node->stf != nullptr) visitStfOp(node->stf);
    else if(node->fcall != nullptr) visitFuncCallExpr(node->fcall);
    else if(node->typecast != nullptr) visitTypecastExprNode(node->typecast);
    
    if(node->op != nullptr) visitIndexOp(node->op);
}
void ygen::ByteCodeGenerator::visitTypecastExprNode(AST::TypecastExprNode *node) {
    visitIdentifier(node->expr);
    visitString(node->type);
    visitAsOp(node->op);
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
        visitIdentifier(node->iden);
        visitExpr(node->expr);
        ASSIGN(1.0)
    }
    else {
        visitIdentifier(node->iden);
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
    else if(node->strexpr != nullptr)
        visitStructExpr(node->strexpr);
    else if(node->newexpr != nullptr)
        visitNewExpr(node->newexpr);
}
void ygen::ByteCodeGenerator::visitFuncCallExpr(AST::FuncCallNode* node) {
    PARAEND
    if(!node->paras.empty()) {
        // 函数调用有参数，生成参数指令
        for(int i = 0; i < node->paras.size(); i ++) {
            visitExpr(node->paras[i]);
        }
    }
    visitIdentifierText(node->iden); // push一个iden的text用于调用函数
    CALL
}
void ygen::ByteCodeGenerator::visitStructExpr(AST::StructExprNode *node) {
    PARAEND
    if(!node->elements.empty()) {
        // 函数调用有参数，生成参数指令
        for(int i = 0; i < node->elements.size(); i ++) {
            visitExpr(node->elements[i]);
        }
    }
    FLAG(ygen::paraHelper::flagt::strtExpr, node->left->line, node->left->column)
}
void ygen::ByteCodeGenerator::visitNewExpr(AST::NewExprNode* node) {
    if(node->initlist == nullptr) {
        PARAEND_LC(node->mark->line, node->mark->column)
        visitIdentifierText(node->iden);
        NEW(node->mark->line, node->mark->column)
    }
    else {
        PARAEND_LC(node->mark->line, node->mark->column)
        if(!node->initlist->elements.empty()) {
            // 函数调用有参数，生成参数指令
            for(int i = 0; i < node->initlist->elements.size(); i ++) {
                visitExpr(node->initlist->elements[i]);
            }
        }
        visitIdentifierText(node->iden);
        NEW(node->mark->line, node->mark->column)
    }
}

// STMT

void ygen::ByteCodeGenerator::visitOutStmt(AST::OutStmtNode* node) {
    visitExpr(node->expr);
    OUT
}
void ygen::ByteCodeGenerator::visitStructDefStmt(AST::StructDefineStmtNode *node) {
    visitString(node->name);
    PARAEND
    if(!node->members.empty()) {
        for(int i = 0; i < node->members.size(); i++) {
            visitString(node->members[i]->name);
            visitString(node->members[i]->type);
        }
    }
    CREATE(addPara(node->name->content), addPara(node->mark->content), 0, 0)
}
void ygen::ByteCodeGenerator::visitVorcStmt(AST::VorcStmtNode* node) {
    for(int i = 0; i < node->defintions.size(); i++) {
        auto def = node->defintions[i];
        if (def->expr == nullptr)
            PUSH(0.0, ytype::type(def->type == nullptr?ytype::basicType::null:ytype::string2BasicType(def->type->content), ytype::norm), node->mark->line, node->mark->column)
        else
            visitExpr(def->expr);
        CREATE(addPara(def->name->content),
               node->modifier != nullptr ? addPara(node->modifier->content):addPara(node->mark->content),
               def->separate != nullptr ? addPara(def->type->content) : 0.0,
               def->separate != nullptr ? 1.0 : 0.0);
    }
}
void ygen::ByteCodeGenerator::visitSpExprStmt(AST::SpExprStmtNode* node) {
    if(node->assign != nullptr) visitAssignmentExpr(node->assign);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
    else if(node->fcall != nullptr) visitFuncCallExpr(node->fcall);
    else if(node->typecast != nullptr) visitTypecastExprNode(node->typecast);
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

    FLAG(paraHelper::flagt::loopEnd, node->mark->line, node->mark->column);
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
    PUSH(-1, ytype::type(ytype::basicType::integer, ytype::norm), node->mark->line, node->mark->column)
    CREATE(addPara("_rit" + std::to_string(repit) + "_"), addPara("var"), addPara("integer"), 1.0); // 创建迭代器，初始值设置为1

    PUSH(addPara("_rit" + std::to_string(repit) + "_"), ytype::type(ytype::basicType::iden, ytype::norm), node->mark->line, node->mark->column)
    visitAddExpr(node->times);
    buildIntegerNumber("1", node->left->line, node->left->column);
    SUB(node->left->line, node->left->column)

    LT(node->right->line, node->right->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    DEL_VAL

    SCOPE_BEGIN{
        PUSH(addPara("_rit" + std::to_string(repit) + "_"), ytype::type(ytype::basicType::string, ytype::norm), node->mark->line,
                   node->mark->column);
        SELF_ADD(0.0, node->left->line, node->left->column)
        visit(node->body->stmts);

        PUSH(addPara("_rit" + std::to_string(repit) + "_"), ytype::type(ytype::basicType::iden, ytype::norm), node->mark->line, node->mark->column)
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
    FLAG(paraHelper::flagt::loopEnd, node->mark->line, node->mark->column);
    repit --;
}

void ygen::ByteCodeGenerator::visitForStmt(AST::ForStmtNode* node) {
    if (node->hasVorc)
        visitVorcStmt(node->vorc);

    if (node->hasCond)
        visitBoolExpr(node->cond);
    else
        PUSH(1.0, ytype::type(ytype::basicType::boolean, ytype::norm), node->mark->line, node->mark->column)
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
            PUSH(1.0, ytype::type(ytype::basicType::boolean, ytype::norm), node->mark->line, node->mark->column)
    }SCOPE_END
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::backScope, node->mark->line, node->mark->column)
    DEL_VAL
    // 释放局部变量
    FLAG(paraHelper::flagt::loopEnd, node->mark->line, node->mark->column);
    if(node->hasVorc)
        DEL(addPara(node->vorc->defintions[0]->name->content), 1.0)
}

void ygen::ByteCodeGenerator::visitDeleteStmt(AST::DeleteStmtNode* node) {
    visitIdentifierText(node->iden);
    DEL(0, 0)
}

void ygen::ByteCodeGenerator::visitBreakStmt(AST::BreakStmtNode* node) {
    JMP(paraHelper::jmpt::unconditional, paraHelper::jmpt::outLoop, node->mark->line, node->mark->column)
}

void ygen::ByteCodeGenerator::visitFuncDefStmt(AST::FuncDefStmtNode* node) {
    PUSH(addPara(node->rettype->content), ytype::type(ytype::basicType::string, ytype::norm), node->mark->line, node->mark->column); // 将要定义的函数的返回值类型入栈
    PARAEND
    if(node->hasPara) {
        for(int i = 0; i < node->paras.size(); i++) {
            PUSH(addPara(node->paras[i]->paratype->content), ytype::type(ytype::basicType::string, ytype::norm), node->paras[i]->paratype->line, node->paras[i]->paratype->column);
            PUSH(addPara(node->paras[i]->paraname->content), ytype::type(ytype::basicType::string, ytype::norm), node->paras[i]->paraname->line, node->paras[i]->paraname->column);
        }
    }
    CREATE(addPara(node->name->content), addPara(node->mark->content), addPara(node->rettype->content), node->hasPara)
    visitBlockStmt(node->body);
    FLAG(paraHelper::flagt::fnEnd, node->mark->line, node->mark->column);
}

AST::StmtNode* ygen::ByteCodeGenerator::visitDeferStmt(AST::DeferStmtNode *node) {
    return node->stmt;
}

void ygen::ByteCodeGenerator::visitReturnStmt(AST::ReturnStmtNode *node) {
    visitExpr(node->expr);
    JMP(paraHelper::jmpt::unconditional, paraHelper::jmpt::outFn, node->mark->line, node->mark->column);
}

void ygen::ByteCodeGenerator::visit(std::vector<AST::StmtNode*> stmts) {
    for(auto stmt: stmts) {
        if(stmt->deferstmt != nullptr) stmts.push_back(visitDeferStmt(stmt->deferstmt));
    }

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
        else if(stmt->sdefstmt != nullptr) visitStructDefStmt(stmt->sdefstmt);
        else if(stmt->retstmt != nullptr) visitReturnStmt(stmt->retstmt);
    }
}
