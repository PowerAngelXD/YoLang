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
#define STF(sname) normalCtor(btc::selfsub, sname, 0, node->name->line, node->name->column);
#define OUT normalCtor(btc::out, 0, 0, node->mark->line, node->mark->column);
#define DEFINE(name) normalCtor(btc::define, name, 0, node->mark->line, node->mark->column);
#define INIT(name, type, vtype, vsign) completeCtor(btc::init, name, type, vtype, vsign, node->mark->line, node->mark->column);
#define ASSIGN(isidx) normalCtor(btc::assign, isidx, 0.0, node->equ->line, node->equ->column);
#define DEL(name, type) normalCtor(btc::del, name, type, node->mark->line, node->mark->column);
#define LISTEND minCtor(btc::listend, node->right->line, node->right->column);
#define PARAEND minCtor(btc::paraend, node->right->line, node->left->column);
#define IDENEND(line, column) minCtor(btc::idenend, line, column);
#define CALL minCtor(btc::call, node->left->line, node->left->column);

ygen::type::modifier norm = 0.0;
ygen::type::modifier list = 0.1;
ygen::type::modifier dict = 0.2;

float ygen::type::type(vtype t, modifier m) {
    return t + m;
}
int ygen::type::getType(vtypeUnit unit) {
    type::vtype t = static_cast<type::vtype>(unit);
    return t;
}
float ygen::type::getModifier(vtypeUnit unit) {
    type::vtype t = static_cast<type::vtype>(unit);
    type::modifier m = unit - t;
    return m;
}

ygen::type::vtype ygen::type::string2Vtype(std::string s) {
    type::vtype t;
    if(s == "integer") t = type::vtype::integer;
    else if(s == "decimal") t = type::vtype::decimal;
    else if(s == "string") t = type::vtype::string;
    else if(s == "object") t = type::vtype::object;
    else if(s == "boolean") t = type::vtype::boolean;
    else if(s == "null") t = type::vtype::null;
    else throw yoexception::YoError("GneratorError", "Wrong string: " + s + "to initialize a vtype!", -1, -1);
    return t;
}
ygen::type::modifier ygen::type::string2Modifier(std::string s) {
    modifier m;
    if(s == "norm") m = norm;
    else if(s == "list") m = list;
    else if(s == "dict") m = dict;
    else throw yoexception::YoError("GneratorError", "Wrong string: " + s + "to initialize a vtype!", -1, -1);
    return m;
}

std::string ygen::type::modifier2String(modifier m) {
    std::string ret;
    if(m == norm) ret = "norm";
    else if(m == list) ret = "list";
    else if(m == dict) ret = "dict";
    return ret;
}
std::string ygen::type::vtype2String(ygen::type::vtype t) {
    std::string ret = "null";
    switch (t) {
        case integer:
            ret = "integer";
            break;
        case boolean:
            ret = "boolean";
            break;
        case decimal:
            ret = "decimal";
            break;
        case string:
            ret = "string";
            break;
        case null:
            ret = "null";
            break;
        case object:
            ret = "obj";
            break;
        default:
            break;
    }
    return ret;
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
        PUSH(0, type::type(type::vtype::boolean, type::norm), token->line, token->column)
    else if(token->content == "true")
        PUSH(1, type::type(type::vtype::boolean, type::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitNumber(yolexer::yoToken* token){
    if(token->type == yolexer::yoTokType::Integer)
        PUSH(atoi(token->content.c_str()), type::type(type::vtype::integer, type::norm), token->line, token->column)
    else if(token->type == yolexer::yoTokType::Decimal)
        PUSH(atof(token->content.c_str()), type::type(type::vtype::decimal, type::norm), token->line, token->column)
}
void ygen::ByteCodeGenerator::buildIntegerNumber(std::string number, int line, int column) {
    PUSH(atoi(number.c_str()), type::type(type::vtype::integer, type::norm), line, column)
}
void ygen::ByteCodeGenerator::buildDecimalNumber(std::string number, int line, int column) {
    PUSH(atof(number.c_str()), type::type(type::vtype::decimal, type::norm), line, column)
}
void ygen::ByteCodeGenerator::visitString(yolexer::yoToken* token) {
    std::string content = token->content;
    std::string ret;
    for (int i = 0; i < content.size(); i++) {
        if (content[i] == '\\') {
            // ????????????
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
    PUSH(addPara(ret), type::type(type::vtype::string, type::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitNull(yolexer::yoToken* token){
    PUSH(0.0, type::type(type::vtype::null, type::norm), token->line, token->column);
}
void ygen::ByteCodeGenerator::visitIdentifier(AST::IdentifierNode* node){
    if(node->dots.empty()){
        PUSH(addPara(node->idens[0]->content), type::type(type::vtype::iden, type::norm), node->idens[0]->line, node->idens[0]->column)
        IDENEND(node->idens[0]->line, node->idens[0]->column)
    }
    else{
        for(int i = 0; i < node->idens.size(); i++) {
            PUSH(addPara(node->idens[i]->content), type::type(type::vtype::iden, type::norm), node->idens[i]->line, node->idens[i]->column);
            if(i == node->idens.size() - 1); // ?????????????????????gmem
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
    PUSH(addPara(text), type::type(type::vtype::string, type::norm), node->idens[0]->line, node->idens[0]->column);
}
void ygen::ByteCodeGenerator::visitSiadExpr(AST::SiadExprNode* node){
    if(node->isFront){
        // ???????????????
        if(node->op->content == "++"){
            visitIdentifierText(node->iden);
            SELF_ADD(1.0, node->op->line, node->op->column) // ???????????????????????????1????????????0
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
        // ??????????????????????????????????????????
        for(int i = 0; i < node->paras.size(); i ++) {
            visitExpr(node->paras[i]);
        }
        PARAEND
    }
    visitIdentifierText(node->iden); // push??????iden???text??????????????????
    CALL
}

// STMT

void ygen::ByteCodeGenerator::visitOutStmt(AST::OutStmtNode* node) {
    visitExpr(node->expr);
    OUT
}
void ygen::ByteCodeGenerator::visitVorcStmt(AST::VorcStmtNode* node) {
    DEFINE(addPara(node->name->content))
    if (node->expr == nullptr)
        PUSH(0.0, type::type(type::vtype::null, type::norm), node->mark->line, node->mark->column)
    else
        visitExpr(node->expr);
    INIT(addPara(node->name->content),
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
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outWscope, node->mark->line, node->mark->column)
    SCOPE_BEGIN{
        visit(node->body->stmts);
        // ???????????????????????????????????????????????????????????????jmp???????????????????????????????????????
        visitBoolExpr(node->cond);
    }
    SCOPE_END
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::findSStart, node->mark->line, node->mark->column)
}
void ygen::ByteCodeGenerator::visitIfStmt(AST::IfStmtNode* node) {
    visitBoolExpr(node->cond);
    JMP(paraHelper::reqTrue, paraHelper::jmpt::outIFscope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}
void ygen::ByteCodeGenerator::visitElifStmt(AST::ElifStmtNode* node) {
    visitBoolExpr(node->cond);
    JMP(paraHelper::reqTrue, paraHelper::jmpt::outElifscope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}
void ygen::ByteCodeGenerator::visitElseStmt(AST::ElseStmtNode* node) {
    JMP(paraHelper::reqTrue, paraHelper::jmpt::outElsescope, node->mark->line, node->mark->column)
    visitBlockStmt(node->body);
}

void ygen::ByteCodeGenerator::visitRepeatStmt(AST::RepeatStmtNode *node) {
    repit ++;
    DEFINE(addPara("__repit" + std::to_string(repit) + "__"))
    PUSH(-1, type::type(type::vtype::integer, type::norm), node->mark->line, node->mark->column)
    INIT(addPara("__repit" + std::to_string(repit) + "__"), addPara("var"), addPara("integer"), 1.0); // ????????????????????????????????????1

    PUSH(addPara("__repit" + std::to_string(repit) + "__"), type::type(type::vtype::iden, type::norm), node->mark->line, node->mark->column)
    IDENEND(node->mark->line, node->mark->column);
    visitAddExpr(node->times);
    buildIntegerNumber("1", node->left->line, node->left->column);
    SUB(node->left->line, node->left->column)

    LT(node->right->line, node->right->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outWscope, node->mark->line, node->mark->column)

    SCOPE_BEGIN{
        PUSH(addPara("__repit" + std::to_string(repit) + "__"), type::type(type::vtype::string, type::norm), node->mark->line,
                   node->mark->column);
        SELF_ADD(0.0, node->left->line, node->left->column)
        visit(node->body->stmts);

        PUSH(addPara("__repit" + std::to_string(repit) + "__"), type::type(type::vtype::iden, type::norm), node->mark->line, node->mark->column)
        IDENEND(node->left->line, node->left->column)
        visitAddExpr(node->times);
        buildIntegerNumber("1", node->left->line, node->left->column);
        SUB(node->left->line, node->left->column)
        LT(node->right->line, node->right->column)
    }
    SCOPE_END

    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::findSStart, node->mark->line, node->mark->column)
    // repit?????????
    DEL(addPara("__repit" + std::to_string(repit) + "__"), 1.0)
    repit --;
}

void ygen::ByteCodeGenerator::visitForStmt(AST::ForStmtNode* node) {
    if (node->hasVorc)
        visitVorcStmt(node->vorc);

    if (node->hasCond)
        visitBoolExpr(node->cond);
    else
        PUSH(1.0, type::type(type::vtype::boolean, type::norm), node->mark->line, node->mark->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outWscope, node->mark->line, node->mark->column)
    SCOPE_BEGIN{
        visit(node->body->stmts);
        if (node->assign != nullptr)
            visitAssignmentExpr(node->assign);
        else if (node->siad != nullptr)
            visitSiadExpr(node->siad);

        if (node->hasCond)
            visitBoolExpr(node->cond);
        else
            PUSH(1.0, type::type(type::vtype::boolean, type::norm), node->mark->line, node->mark->column)
    }SCOPE_END
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::findSStart, node->mark->line, node->mark->column)
    // ??????????????????
    if(node->hasVorc)
        DEL(addPara(node->vorc->name->content), 1.0)
}

void ygen::ByteCodeGenerator::visitDeleteStmt(AST::DeleteStmtNode* node) {
    visitIdentifierText(node->iden);
    DEL(0, 0)
}

void ygen::ByteCodeGenerator::visitBreakStmt(AST::BreakStmtNode* node) {
    PUSH(0.0, type::type(type::vtype::boolean, type::norm), node->mark->line, node->mark->column)
    JMP(paraHelper::jmpt::reqTrue, paraHelper::jmpt::outScopeDirectly, node->mark->line, node->mark->column)
}

void ygen::ByteCodeGenerator::visitFuncDefStmt(AST::FuncDefStmtNode* node) {
    PUSH(addPara(node->rettype->content), type::type(type::vtype::string, type::norm), node->mark->line, node->mark->column); // ?????????????????????????????????????????????
    PARAEND // ?????????????????????flag????????????
    if(node->hasPara) {
        for(int i = 0; i < node->paras.size(); i++) {
            PUSH(addPara(node->paras[i]->paratype->content), type::type(type::vtype::string, type::norm), node->paras[i]->paratype->line, node->paras[i]->paratype->column);
            PUSH(addPara(node->paras[i]->paraname->content), type::type(type::vtype::string, type::norm), node->paras[i]->paraname->line, node->paras[i]->paraname->column);
        }
    }
    DEFINE(addPara(node->name->content)) // ????????????
    INIT(addPara(node->name->content), addPara(node->mark->content), addPara(node->rettype->content), 1.0)
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
