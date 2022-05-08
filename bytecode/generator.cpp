#include "generator.h"

ygen::ByteCodeGenerator::ByteCodeGenerator(AST::WholeExprNode* expr) {
    this->expr = *expr;
}
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
void ygen::ByteCodeGenerator::genFile() {
    std::ofstream file("yo.ybtc");
    
    std::string paraArea;
    for(int i = 0; i < codes.size(); i++) {
        file<<removeZero(codes[i].code)<<",";
        file<<removeZero(codes[i].arg1)<<",";
        file<<removeZero(codes[i].arg2)<<",";
        file<<removeZero(codes[i].arg3)<<",";
        file<<removeZero(codes[i].arg4)<<"\n";
    }
    file<<"9,9,9,9,9\n#"; // 字节码终止为9999
    for(auto para: constpool) {
        file<<para<<",";
    }
    file.close();
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
        normalCtor(btc::push, addPara(ret), paraHelper::charcter, token->line, token->column);
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
void ygen::ByteCodeGenerator::visitSiadExpr(AST::SiadExprNode* node){
    if(node->isFront){
        // 是前置类型
        if(node->op->content == "++"){
            visitIdentifier(node->iden);
            normalCtor(btc::selfadd, 1.0, 0.0, node->op->line, node->op->column); // 前置第一个参数为1，否则为0
        }
        else if(node->op->content == "--"){
            visitIdentifier(node->iden);
            normalCtor(btc::selfsub, 1.0, 0.0, node->op->line, node->op->column);
        }
    }
    else{
        if(node->op->content == "++"){
            visitIdentifier(node->iden);
            normalCtor(btc::selfadd, 0.0, 0.0, node->op->line, node->op->column);
        }
        else if(node->op->content == "--"){
            visitIdentifier(node->iden);
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
void ygen::ByteCodeGenerator::visitPrimExpr(AST::PrimExprNode* node){
    if(node->number != nullptr) visitNumber(node->number);
    else if(node->string != nullptr) visitStrCh(node->string);
    else if(node->character != nullptr) visitStrCh(node->character);
    else if(node->null != nullptr) visitNull(node->null);
    else if(node->iden != nullptr) visitIdentifier(node->iden);
    else if(node->siad != nullptr) visitSiadExpr(node->siad);
    else if(node->expr != nullptr) visitExpr(node->expr);
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
    minCtor(btc::lst, node->left->line, node->left->column);
}
void ygen::ByteCodeGenerator::visitExpr(AST::WholeExprNode* node){
    if(node->addexpr != nullptr)
        visitAddExpr(node->addexpr);
    else if(node->boolexpr != nullptr)
        visitBoolExpr(node->boolexpr);
    else if(node->listexpr != nullptr)
        visitListExpr(node->listexpr);
}