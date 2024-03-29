#include "generator.h"

#include <utility>
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
#define TYPE_EQU(line, column) normalCtor(btc::type_equ, 0, 0, line, column);
#define NOEQU(line, column) normalCtor(btc::noequ, 0, 0, line, column);
#define LOGIC_AND normalCtor(btc::logicand, 0, 0, node->op->line, node->op->column);
#define LOGIC_OR normalCtor(btc::logicor, 0, 0, node->op->line, node->op->column);
#define NO normalCtor(btc::no, 0, 0, node->op->line, node->op->column);
#define STF(sname) normalCtor(btc::stf, sname, 0, node->name->line, node->name->column);
#define OUT normalCtor(btc::out, 0, 0, node->mark->line, node->mark->column);
#define CREATE(name, type, basicType, vsign) completeCtor(btc::create, name, type, basicType, vsign, node->mark->line, node->mark->column);
#define ASSIGN normalCtor(btc::assign, 0.0, 0.0, node->equ->line, node->equ->column);
#define POINT_TO normalCtor(btc::point_to, 0.0, 0.0, node->equ->line, node->equ->column);
#define DEL(name, type) normalCtor(btc::del, name, type, node->mark->line, node->mark->column);
#define LISTEND minCtor(btc::listend, node->right->line, node->right->column);
#define PARAEND minCtor(btc::paraend, node->right->line, node->left->column);
#define PARAEND_LC(line, column) minCtor(btc::paraend, line, column);
#define TCAST minCtor(btc::tcast, node->op->line, node->op->column);
#define IDENEND(line, column) minCtor(btc::idenend, line, column);
#define CALL minCtor(btc::call, node->left->line, node->left->column);
#define DEL_VAL minCtor(btc::del_val, node->mark->line, node->mark->column);
#define FLAG(type, line, column) normalCtor(btc::set_flag, type, 0.0, line, column);
#define NEW(line, column) normalCtor(btc::_new, 0, 0, line, column);

using namespace ygen;

btc ygen::string2Code(std::string s) {
    btc code;
    if(s=="jmp") code=btc::jmp;
    else if(s=="push") code=btc::push;else if(s=="gto") code=btc::gto;else if(s=="tcast") code=btc::tcast;
    else if(s=="cast") code=btc::cast;else if(s=="selfadd") code=btc::selfadd;else if(s=="selfsub") code=btc::selfsub;
    else if(s=="add") code=btc::add;else if(s=="lt") code=btc::lt;else if(s=="scopestart") code=btc::scopestart;
    else if(s=="sub") code=btc::sub;else if(s=="ltet") code=btc::ltet;else if(s=="scopeend") code=btc::scopeend;
    else if(s=="mul") code=btc::mul;else if(s=="gt") code=btc::gt;else if(s=="idenend") code=btc::idenend;
    else if(s=="div") code=btc::div;else if(s=="gtet") code=btc::gtet;else if(s=="flag") code=btc::set_flag;
    else if(s=="tmo") code=btc::tmo;else if(s=="equ") code=btc::equ;else if(s=="out") code=btc::out;
    else if(s=="idx") code=btc::idx;else if(s=="noequ") code=btc::noequ;else if(s=="create") code=btc::create;
    else if(s=="lst") code=btc::lst;else if(s=="gmem") code=btc::gmem;else if(s=="assign") code=btc::assign;
    else if(s=="logicand") code=btc::logicand;else if(s=="stf") code=btc::stf;else if(s=="del") code=btc::del;
    else if(s=="logicor") code=btc::logicor;else if(s=="listend") code=btc::listend;else if(s=="call") code=btc::call;
    else if(s=="no") code=btc::no;else if(s=="paraend") code=btc::paraend;else if(s=="del_val") code=btc::del_val;
    else if(s=="new") code=btc::_new;else if(s=="type_equ") code=btc::type_equ; else if(s=="poinnt_to")code=btc::point_to;
    return code;
}


byteCodeGenerator::byteCodeGenerator(std::vector<AST::StmtNode*> _stmts): stmts(std::move(_stmts)) {}
byteCodeGenerator::byteCodeGenerator(AST::WholeExprNode* _expr): expr(_expr) {}
std::string byteCodeGenerator::removeZero(float content) {
    std::ostringstream oss;
    oss<<content;
    return oss.str();
}
int byteCodeGenerator::string2Int(const std::string& s) {
    std::stringstream ss;
    ss<<s;
    int ret;
    ss>>ret;
    return ret;
}
float byteCodeGenerator::string2Float(const std::string& s) {
    std::stringstream ss;
    ss<<s;
    float ret;
    ss>>ret;
    return ret;
}
std::vector<std::string> byteCodeGenerator::getConstPool() {
    return constpool;
}
std::vector<byteCode> byteCodeGenerator::getCodes() {
    return codes;
}
int byteCodeGenerator::addPara(std::string para){
    constpool.push_back(para);
    return static_cast<int>(constpool.size())-1;
}
void byteCodeGenerator::minCtor(btc code, int ln, int col){
    codes.push_back({code, {ytype::basicType::null, ytype::compType::norm}, 0.0, 0.0, 0.0, 0.0, ln, col});
}
void byteCodeGenerator::normalCtor(btc code, float arg1, float arg2, int ln, int col){
    codes.push_back({code, {ytype::basicType::null, ytype::compType::norm}, arg1, arg2, 0.0, 0.0, ln, col});
}
void byteCodeGenerator::completeCtor(btc code, float arg1, float arg2, float arg3, float arg4, int ln, int col){
    codes.push_back({code, {ytype::basicType::null, ytype::compType::norm}, arg1, arg2, arg3, arg4, ln, col});
}
void byteCodeGenerator::pushCtor(float arg1, ytype::ytypeUnit t, float arg2, int ln, int col) {
    codes.push_back({btc::push, t, arg1, arg2, 0.0, 0.0, ln, col});
}

void byteCodeGenerator::visitBoolean(yolexer::yoToken* token){
    if(token->content == "false")
        PUSH(0, ytype::type(ytype::basicType::boolean, ytype::norm), token->line, token->column)
    else if(token->content == "true")
        PUSH(1, ytype::type(ytype::basicType::boolean, ytype::norm), token->line, token->column)
}
void byteCodeGenerator::visitNumber(yolexer::yoToken* token){
    if(token->type == yolexer::yoTokType::Integer)
        PUSH(string2Float(token->content), ytype::type(ytype::basicType::integer, ytype::norm), token->line, token->column)
    else if(token->type == yolexer::yoTokType::Decimal)
        PUSH(string2Float(token->content), ytype::type(ytype::basicType::decimal, ytype::norm), token->line, token->column)
}
void byteCodeGenerator::buildIntegerNumber(std::string number, int line, int column) {
    PUSH(string2Float(number), ytype::type(ytype::basicType::integer, ytype::norm), line, column)
}
void byteCodeGenerator::buildDecimalNumber(std::string number, int line, int column) {
    PUSH(string2Float(number), ytype::type(ytype::basicType::decimal, ytype::norm), line, column)
}
void byteCodeGenerator::visitString(yolexer::yoToken* token) {
    PUSH(static_cast<float>(addPara(token->content)), ytype::type(ytype::basicType::string, ytype::norm), token->line, token->column)
}
void byteCodeGenerator::visitNull(yolexer::yoToken* token){
    PUSH(0.0, ytype::type(ytype::basicType::null, ytype::norm), token->line, token->column)
}
void byteCodeGenerator::visitCallOp(AST::CallOpNode* node) {
    PARAEND
    if(!node->paras.empty()) {
        // 函数调用有参数，生成参数指令
        for(auto & para : node->paras) {
            visitExpr(para);
        }
    }
    CALL
}
void byteCodeGenerator::visitCellIdentifier(AST::CellIdentifierNode* node) {
    PUSH(static_cast<float>(addPara(node->iden->content)), ytype::type(ytype::basicType::iden, ytype::norm), node->iden->line, node->iden->column)
    if(node->call != nullptr) visitCallOp(node->call);
}
void byteCodeGenerator::visitIdentifier(AST::IdentifierNode *node) {
    visitCellIdentifier(node->iden);
    if(node->idx != nullptr) visitIndexOp(node->idx);
}
void byteCodeGenerator::visitIdentifierExpr(AST::IdentifierExprNode* node){
    if(node->dots.empty()){
        visitIdentifier(node->idens[0]);
    }
    else{
        visitIdentifier(node->idens[0]);
        for(int i = 1; i < node->idens.size(); i++) {
            visitIdentifierText(node->idens[i]);
            GMEM
        }
    }
}
void byteCodeGenerator::visitIdentifierText(AST::IdentifierNode* node) {
    PUSH(static_cast<float>(addPara(node->iden->iden->content)), ytype::type(ytype::basicType::string, ytype::norm), node->iden->iden->line, node->iden->iden->column)
}
void byteCodeGenerator::visitIdentifierExprText(AST::IdentifierExprNode* node, bool isref){
    std::string text;
    text += node->idens[0]->iden->iden->content;
    for(unsigned long long int i = 0; i < node->dots.size(); i++) {
        text += node->dots[i]->content;
        text += node->idens[static_cast<unsigned long long int>(i + 1)]->iden->iden->content;
    }
    PUSH(static_cast<float>(addPara(text)), ytype::type(ytype::basicType::string, ytype::norm), node->idens[0]->iden->iden->line, node->idens[0]->iden->iden->column)
}
void byteCodeGenerator::visitSiadExpr(AST::SiadExprNode* node){
    if(node->isFront){
        // 是前置类型
        if(node->op->content == "++"){
            visitIdentifierExpr(node->iden);
            SELF_ADD(1.0, node->op->line, node->op->column) // 如果是前置则参数为1，否则为0
        }
        else if(node->op->content == "--"){
            visitIdentifierExpr(node->iden);
            SELF_SUB(1.0)
        }
    }
    else{
        if(node->op->content == "++"){
            visitIdentifierExpr(node->iden);
            SELF_ADD(0.0, node->op->line, node->op->column)
        }
        else if(node->op->content == "--"){
            visitIdentifierExpr(node->iden);
            SELF_SUB(0.0)
        }
    }
}
void byteCodeGenerator::visitAddOp(AST::AddOpNode* node){
    if(node->op->content == "+")
        ADD(node->op->line, node->op->column)
    else if(node->op->content == "-") SUB(node->op->line, node->op->column)
}
void byteCodeGenerator::visitMulOp(AST::MulOpNode* node){
    if(node->op->content == "*")
        MUL(node->op->line, node->op->column)
    else if(node->op->content == "/")
        DIV(node->op->line, node->op->column)
    else if(node->op->content == "%")
        TMO
}
void byteCodeGenerator::visitAsOp(AST::AsOpNode *node) {
    TCAST
}
void byteCodeGenerator::visitCmpOp(AST::CmpOpNode* node){
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
    else if(node->op->content == "<=>")
        TYPE_EQU(node->op->line, node->op->column)
    else if(node->op->content == "!")
        NO
}
void byteCodeGenerator::visitBoolOp(AST::BoolOpNode* node){
    if(node->op->content == "&&")
        LOGIC_AND
    else if(node->op->content == "||")
        LOGIC_OR
}
void byteCodeGenerator::visitIndexOp(AST::IndexOpNode* node){
    visitAddExpr(node->index);
    IDX
}
void byteCodeGenerator::visitStfOp(AST::StfOpNode* node){
    if(node->expr != nullptr) visitExpr(node->expr);
    STF(static_cast<float>(addPara(node->name->content)))
}
void byteCodeGenerator::visitCellExpr(AST::CellExprNode *node) {
    if(node->op->content == "+");
    else if(node->op->content == "-"){
        buildIntegerNumber("0", node->op->line, node->op->column);
        visitPrimExpr(node->expr);
        SUB(node->op->line, node->op->column)
    }
}
void byteCodeGenerator::visitPrimExpr(AST::PrimExprNode* node){
    if(node->number != nullptr) visitNumber(node->number);
    else if(node->cellexpr != nullptr) visitCellExpr(node->cellexpr);
    else if(node->string != nullptr) visitString(node->string);
    else if(node->boolconst != nullptr) visitBoolean(node->boolconst);
    else if(node->null != nullptr) visitNull(node->null);
    else if(node->iden != nullptr) visitIdentifierExpr(node->iden);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
    else if(node->expr != nullptr) visitExpr(node->expr);
    else if(node->stf != nullptr) visitStfOp(node->stf);
    else if(node->typecast != nullptr) visitTypecastExprNode(node->typecast);
}
void byteCodeGenerator::visitTypecastExprNode(AST::TypecastExprNode *node) {
    visitIdentifierExpr(node->expr);
    visitString(node->type);
    visitAsOp(node->op);
}
void byteCodeGenerator::visitMulExpr(AST::MulExprNode* node){
    visitPrimExpr(node->prims[0]);
    for(unsigned long long int i = 0; i < node->ops.size(); i++) {
        visitPrimExpr(node->prims[(i + 1)]);
        visitMulOp(node->ops[i]);
    }
}
void byteCodeGenerator::visitAddExpr(AST::AddExprNode* node){
    visitMulExpr(node->muls[0]);
    for(unsigned long long int i = 0; i < node->ops.size(); i++) {
        visitMulExpr(node->muls[i + 1]);
        visitAddOp(node->ops[i]);
    }
}
void byteCodeGenerator::visitCmpExpr(AST::CmpExprNode* node){
    visitAddExpr(node->expr);
    if(node->op != nullptr){
        visitAddExpr(node->cmpExpr);
        visitCmpOp(node->op);
    }
    if(node->reverse != nullptr)
        visitCmpOp(node->reverse);
}
void byteCodeGenerator::visitBoolExpr(AST::BoolExprNode* node){
    visitCmpExpr(node->cmps[0]);
    for(unsigned long long int i = 0; i < node->ops.size(); i++) {
        visitCmpExpr(node->cmps[i + 1]);
        visitBoolOp(node->ops[i]);
    }
}
void byteCodeGenerator::visitListExpr(AST::ListExprNode* node){
    LISTEND
    for(int i = 0; i < node->elements.size(); i++) {
        visitExpr(node->elements[i]);
    }
    LST
}
void byteCodeGenerator::visitAssignmentExpr(AST::AssignmentExprNode* node){
    if(node->equ->content == "=") {
        visitIdentifierExpr(node->iden);
        visitExpr(node->expr);
        ASSIGN
    }
    else if(node->equ->content == "+=") {
        visitIdentifierExpr(node->iden);
        {
            visitIdentifierExpr(node->iden);
            visitExpr(node->expr);
            ADD(node->equ->line, node->equ->column)
        }
        ASSIGN
    }
    else if(node->equ->content == "-=") {
        visitIdentifierExpr(node->iden);
        {
            visitIdentifierExpr(node->iden);
            visitExpr(node->expr);
            SUB(node->equ->line, node->equ->column)
        }
        ASSIGN
    }
    else if(node->equ->content == "*=") {
        visitIdentifierExpr(node->iden);
        {
            visitIdentifierExpr(node->iden);
            visitExpr(node->expr);
            MUL(node->equ->line, node->equ->column)
        }
        ASSIGN
    }
    else if(node->equ->content == "/=") {
        visitIdentifierExpr(node->iden);
        {
            visitIdentifierExpr(node->iden);
            visitExpr(node->expr);
            DIV(node->equ->line, node->equ->column)
        }
        ASSIGN
    }
    
}
void byteCodeGenerator::visitExpr(AST::WholeExprNode* node){
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

void byteCodeGenerator::visitStructExpr(AST::StructExprNode *node) {
    PARAEND
    if(!node->elements.empty()) {
        // 函数调用有参数，生成参数指令
        for(unsigned long long int i = 0; i < node->elements.size(); i ++) {
            visitExpr(node->elements[i]);
        }
    }
    FLAG(ygen::paraHelper::flagt::strtExpr, node->left->line, node->left->column)
}
void byteCodeGenerator::visitNewExpr(AST::NewExprNode* node) {
    if(node->initlist == nullptr) {
        PARAEND_LC(node->mark->line, node->mark->column)
        visitIdentifierExprText(node->iden);
        NEW(node->mark->line, node->mark->column)
    }
    else {
        PARAEND_LC(node->mark->line, node->mark->column)
        if(!node->initlist->elements.empty()) {
            // 函数调用有参数，生成参数指令
            for(int i = 0; i < node->initlist->elements.size(); i ++) {
                visitExpr(node->initlist->elements[static_cast<unsigned long long int>(i)]);
            }
        }
        visitIdentifierExprText(node->iden);
        NEW(node->mark->line, node->mark->column)
    }
}

// STMT

void byteCodeGenerator::visitStructDefStmt(AST::StructDefineStmtNode *node) {
    visitString(node->name);
    PARAEND
    if(!node->members.empty()) {
        for(auto & member : node->members) {
            visitString(member->name);
            visitString(member->type);
        }
    }
    CREATE(addPara(node->name->content), addPara(node->mark->content), 0, 0)
}
void byteCodeGenerator::visitLetStmt(AST::LetStmtNode* node) {
    for(int i = 0; i < node->defintions.size(); i++) {
        auto def = node->defintions[i];
        if (def->expr == nullptr) {
            if (def->type == nullptr) {
                throw yoexception::YoError("SyntaxError", "At least one type qualifier is required", node->mark->line, node->mark->column);
            }
            PUSH(
                    0.0,
                    ytype::type(ytype::string2BasicType(def->type->content), ytype::norm),
                    node->mark->line,
                    node->mark->column
            )
        }
        else
            visitExpr(def->expr);
        CREATE(addPara(def->name->content),
               node->modifier != nullptr ? addPara(node->modifier->content):addPara(node->mark->content),
               def->separate != nullptr ? addPara(def->type->content) : 0.0,
               def->separate != nullptr ? 1.0 : 0.0)
    }
}
void byteCodeGenerator::visitSpExprStmt(AST::SpExprStmtNode* node) {
    if(node->assign != nullptr) visitAssignmentExpr(node->assign);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
    else if(node->iden != nullptr) visitIdentifierExpr(node->iden);
    else if(node->typecast != nullptr) visitTypecastExprNode(node->typecast);
}
void byteCodeGenerator::visitBlockStmt(AST::BlockStmtNode* node) {
    SCOPE_BEGIN
    visit(node->stmts);
    SCOPE_END
}
void byteCodeGenerator::visitWhileStmt(AST::WhileStmtNode* node) {
    visitBoolExpr(node->cond);
    JMP(paraHelper::jmpt::reqFalse, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    DEL_VAL
    FLAG(paraHelper::flagt::loopEnd, node->mark->line, node->mark->column)
    SCOPE_BEGIN{
        visit(node->body->stmts);
        // 下面的指令用于检测表达式是否还成立，成立则jmp到下一个循环，否则向前偏移
        visitBoolExpr(node->cond);
    }
    SCOPE_END
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::backScope, node->mark->line, node->mark->column)
}
void byteCodeGenerator::visitIfStmt(AST::IfStmtNode* node) {
    visitBoolExpr(node->cond);
    JMP(paraHelper::reqFalse, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}
void byteCodeGenerator::visitElifStmt(AST::ElifStmtNode* node) {
    visitBoolExpr(node->cond);
    JMP(paraHelper::reqFalse, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}
void byteCodeGenerator::visitElseStmt(AST::ElseStmtNode* node) {
    JMP(paraHelper::reqFalse, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}

void byteCodeGenerator::visitRepeatStmt(AST::RepeatStmtNode *node) {
    repit ++;
    PUSH(-1, ytype::type(ytype::basicType::integer, ytype::norm), node->mark->line, node->mark->column)
    CREATE(addPara("_rit" + std::to_string(repit) + "_"), addPara("var"), addPara("integer"), 1.0) // 创建迭代器，初始值设置为1

    PUSH(addPara("_rit" + std::to_string(repit) + "_"), ytype::type(ytype::basicType::iden, ytype::norm), node->mark->line, node->mark->column)
    visitAddExpr(node->times);
    buildIntegerNumber("1", node->left->line, node->left->column);
    SUB(node->left->line, node->left->column)

    LT(node->right->line, node->right->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    DEL_VAL

    FLAG(paraHelper::flagt::loopEnd, node->mark->line, node->mark->column)
    SCOPE_BEGIN{
        PUSH(addPara("_rit" + std::to_string(repit) + "_"), ytype::type(ytype::basicType::iden, ytype::norm), node->mark->line,
                   node->mark->column)
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
    repit --;
}

void byteCodeGenerator::visitForStmt(AST::ForStmtNode* node) {
    if (node->hasVorc)
        visitLetStmt(node->vorc);

    if (node->hasCond)
        visitBoolExpr(node->cond);
    else
        PUSH(1.0, ytype::type(ytype::basicType::boolean, ytype::norm), node->mark->line, node->mark->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outScope, node->mark->line, node->mark->column)
    DEL_VAL

    FLAG(paraHelper::flagt::loopEnd, node->mark->line, node->mark->column)
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
    FLAG(paraHelper::flagt::loopEnd, node->mark->line, node->mark->column)
    if(node->hasVorc)
        DEL(addPara(node->vorc->defintions[0]->name->content), 1.0)
}

void byteCodeGenerator::visitDeleteStmt(AST::DeleteStmtNode* node) {
    visitIdentifierExprText(node->iden);
    DEL(0, 0)
}

void byteCodeGenerator::visitBreakStmt(AST::BreakStmtNode* node) {
    JMP(paraHelper::jmpt::unconditional, paraHelper::jmpt::outLoop, node->mark->line, node->mark->column)
}

void byteCodeGenerator::visitFuncDefStmt(AST::FuncDefStmtNode* node) {
    PUSH(addPara(node->rettype->content), ytype::type(ytype::basicType::string, ytype::norm), node->mark->line, node->mark->column) // 将要定义的函数的返回值类型入栈
    PARAEND
    if(node->hasPara) {
        for(int i = 0; i < node->paras.size(); i++) {
            PUSH(addPara(node->paras[i]->paratype->content), ytype::type(ytype::basicType::string, ytype::norm),
                 node->paras[i]->paratype->line, node->paras[i]->paratype->column)
            PUSH(addPara(node->paras[i]->paraname->content), ytype::type(ytype::basicType::string, ytype::norm),
                 node->paras[i]->paraname->line, node->paras[i]->paraname->column)
        }
    }
    CREATE(addPara(node->name->content), addPara(node->mark->content), addPara(node->rettype->content), node->hasPara)
    visitBlockStmt(node->body);
    FLAG(paraHelper::flagt::fnEnd, node->mark->line, node->mark->column)
}

AST::StmtNode* byteCodeGenerator::visitDeferStmt(AST::DeferStmtNode *node) {
    return node->stmt;
}

void byteCodeGenerator::visitReturnStmt(AST::ReturnStmtNode *node) {
    visitExpr(node->expr);
    JMP(paraHelper::jmpt::unconditional, paraHelper::jmpt::outFn, node->mark->line, node->mark->column)
}

void byteCodeGenerator::visit(std::vector<AST::StmtNode*> _stmts) {
    for(auto stmt: _stmts) {
        if(stmt->deferstmt != nullptr) _stmts.push_back(visitDeferStmt(stmt->deferstmt));
    }

    for(auto stmt: _stmts){
        if(stmt->blockstmt != nullptr) visitBlockStmt(stmt->blockstmt);
        else if(stmt->vorcstmt != nullptr) visitLetStmt(stmt->vorcstmt);
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
