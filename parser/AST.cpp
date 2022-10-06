#include "AST.h"

std::string AST::CallOpNode::toString() {
    std::string ret = "FuncCallNode: {";
    if(dots.empty()) return "FuncCallNode: {" + paras[0]->toString() + (paras.empty()?"}":paras[0]->toString() + "}");
    else{
        ret += "[" + paras[0]->toString() + ", ";
        for(int i = 1; i < dots.size(); i++){
            if(i == dots.size() - 1) break;
            else ret += ", ";
        }
        ret += "] }";
        return ret;
    }
}

std::string AST::StfOpNode::toString() {
    return "StfOpNode: {" + expr->toString() + "}";
}

std::string AST::CellExprNode::toString() {
    return "CellExprNode: {" + op->toString() + ", " + expr->toString() + "}";
}

std::string AST::IdentifierExprNode::toString() {
    if(dots.empty()) return "IdentifierExprNode: {" + idens[0]->toString() + "}";
    else{
        std::string ret = "IdentifierExprNode: { [";
        ret += idens[0]->toString() + ", ";
        for(int i = 0; i < dots.size(); i++){
            ret += idens[i + 1]->toString();
            if(i == dots.size() - 1)break;
            else ret += ", ";
        }
        ret += "] }";
        return ret;
    }
}

std::string AST::MulExprNode::toString() {
    if(ops.empty()) return "MulExprNode: {" + prims[0]->toString() + "}";
    else{
        std::string ret = "MulExprNode: { [";
        ret += prims[0]->toString() + ", ";
        for(int i = 0; i < ops.size(); i++){
            ret += ops[i]->toString();
            ret += ", ";
            ret += prims[i + 1]->toString();
            if(i == ops.size() - 1)break;
            else ret += ", ";
        }
        ret += "] }";
        return ret;
    }
}

std::string AST::AddExprNode::toString() {
    if(ops.empty()) return "AddExprNode: {" + muls[0]->toString() + "}";
    else{
        std::string ret = "AddExprNode: { [";
        ret += muls[0]->toString() + ", ";
        for(int i = 0; i < ops.size(); i++){
            ret += ops[i]->toString();
            ret += ", ";
            ret += muls[i + 1]->toString();
            if(i == ops.size() - 1) break;
            else ret += ", ";
        }
        ret += "] }";
        return ret;
    }
}

std::string AST::BoolExprNode::toString() {
    if(ops.empty()) return "BoolExprNode: {" + cmps[0]->toString() + "}";
    else{
        std::string ret = "BoolExprNode: { [";
        ret += cmps[0]->toString() + ", ";
        for(int i = 0; i < ops.size(); i++){
            ret += ops[i]->toString();
            ret += ", ";
            ret += cmps[i + 1]->toString();
            if(i == ops.size() - 1) break;
            else ret += ", ";
        }
        ret += "] }";
        return ret;
    }
}

std::string AST::ListExprNode::toString() {
    if(dots.empty()) return "ListExprNode: {" + elements[0]->toString() + "}";
    else{
        std::string ret = "ListExprNode: { [";
        ret += elements[0]->toString() + ", ";
        for(int i = 0; i < dots.size(); i++){
            ret += elements[i + 1]->toString();
            if(i == dots.size() - 1) break;
            else ret += ", ";
        }
        ret += "] }";
        return ret;
    }
}

std::string AST::CmpExprNode::toString() {
    return "CmpExprNode: {" + (reverse != nullptr ? reverse->toString() : "") + expr->toString() + 
            (op != nullptr?op->toString() + ", " + cmpExpr->toString() + "}":"}"); 
}

std::string AST::PrimExprNode::toString() {
    std::string ret = "PrimExprNode: {";
    if(left != nullptr){
        ret += expr->toString() + "}";
        return ret;
    }
    else{
        if(number != nullptr) ret += number->toString() + ", ";
        else if(string != nullptr) ret += string->toString() + ", ";
        else if(iden != nullptr) ret += iden->toString() + ", ";
        else if(siad != nullptr) ret += siad->toString() + ", ";
        else if(stf != nullptr) ret += stf->toString() + ", ";
        else if(boolconst != nullptr) ret += boolconst->toString() + ", ";
        else if(typecast != nullptr) ret += typecast->toString() + ", ";
        else if(cellexpr != nullptr) ret += cellexpr->toString() + ", ";

        if(op != nullptr) ret += op->toString() + "}";
        else ret += "}";
        return ret;
    }
}

std::string AST::WholeExprNode::toString() {
    if(addexpr != nullptr) return "WholeExprNode: {" + addexpr->toString() + "}";
    else if(boolexpr != nullptr) return "WholeExprNode: {" + boolexpr->toString() + "}";
    else if(listexpr != nullptr) return "WholeExprNode: {" + listexpr->toString() + "}";
    else if(strexpr != nullptr) return "WholeExprNode: {" + strexpr->toString() + "}";
    else if(assign != nullptr) return "WholeExprNode: {" + assign->toString() + "}";
    else if(newexpr != nullptr) return "WholeExprNode: {" + newexpr->toString() + "}";
    else return "null";
}

std::string AST::AssignmentExprNode::toString() {
    return "AssignmentExprNode: {" + iden->toString() + "}";
}



std::string AST::SiadExprNode::toString(){
    return "SiadExprNode: {" + iden->toString() + ", " + op->toString() + "}";
}

std::string AST::IndexOpNode::toString(){
    return "IndexOpNode: {" + index->toString() + "}";
}

std::string AST::AddOpNode::toString(){
    return op->toString();
}

std::string AST::MulOpNode::toString(){
    return op->toString();
}

std::string AST::CmpOpNode::toString(){
    return op->toString();
}

std::string AST::BoolOpNode::toString(){
    return op->toString();
}

std::string AST::TypecastExprNode::toString() {
    return "TypecastExprNode: {" + expr->toString() + ", " + type->toString() + "}";
}

// STMT

std::string AST::OutStmtNode::toString() {
    return "OutStmtNode: {" + mark->toString() + ", " + expr->toString() + "}";
}

std::string AST::VorcStmtNode::defineBlock::toString() {
    return "DefineBlock: {" + name->toString() + ", " + (type != nullptr?type->toString():"") + ", " + expr->toString() + "}";
}

std::string AST::VorcStmtNode::toString() {
    std::string ret = "VorcStmtNode: {" +(modifier == nullptr?"":modifier->toString()) + mark->toString() + ", [";
    ret += defintions[0]->toString();
    for(int i = 0; i < dots.size(); i ++) {
        ret + ", ";
        ret += defintions[i + 1]->toString();
    }
    ret + "]}";
    return ret;
}

std::string AST::BlockStmtNode::toString() {
    std::string ret = "BlockStmtNode: {[";
    for(auto stmt: stmts) {
        ret += stmt->toString();
    }
    ret += "]}";
    return ret;
}

std::string AST::SpExprStmtNode::toString() {
    if(siad != nullptr) return "SpExprStmtNode: {" + siad->toString() + "}";
    else if(assign != nullptr) return "SpExprStmtNode: {" + assign->toString() + "}";
    else if(iden != nullptr) return "SpExprStmtNode: {" + iden->toString() + "}";
    else if(typecast != nullptr) return "SpExprStmtNode: {" + typecast->toString() + "}";
    else return "null";
}

std::string AST::WhileStmtNode::toString() {
    return "WhileStmtNode: {" + cond->toString() + ", " + body->toString() + "}";
}

std::string AST::IfStmtNode::toString() {
    return "IfStmtNode: {" + cond->toString() + ", " + body->toString() + "}";
}

std::string AST::ElifStmtNode::toString() {
    return "ElifStmtNode: {" + cond->toString() + ", " + body->toString() + "}";
}

std::string AST::ElseStmtNode::toString() {
    return "ElseStmtNode: {" + body->toString() + "}";
}

std::string AST::DeleteStmtNode::toString() {
    return "DeleteStmtNode: {" + iden->toString() + "}";
}

std::string AST::RepeatStmtNode::toString() {
    return "RepeatStmtNode: {" + times->toString() + ", " + body->toString() + "}";
}

std::string AST::ForStmtNode::toString() {
    return "ForStmtNode: {" + (hasVorc?vorc->toString() + ", ":"") + (hasCond?cond->toString() + ", ":"") + (hasOp?(siad != nullptr?siad->toString():assign->toString() + "}"):"}");
}

std::string AST::FuncDefStmtNode::toString() {
    std::string ret = "FuncDefStmtNode: {" + name->toString() + ", ";
    if(hasPara) {
        ret += "[";
        for(int i = 0; i < paras.size() ;i++)
            ret += paras[i]->paraname->toString() + ":" + paras[i]->paratype->toString() + ", ";
        ret += "]";
        return ret + ", " + body->toString() + "}";
    }
    else {
        return ret + body->toString() + "}";
    }
}

std::string AST::ReturnStmtNode::toString() {
    return "ReturnStmtNode: {" + expr->toString() + "}";
}

std::string AST::BreakStmtNode::toString() {
    return "BreakStmtNode";
}

std::string AST::DeferStmtNode::toString() {
    return "DeferStmtNode: {" + stmt->toString() + "}";
}

std::string AST::AsOpNode::toString() {
    return op->toString();
}

std::string AST::StmtNode::toString() {
    if(outstmt != nullptr) return "StmtNode: {" + outstmt->toString() + "}";
    else if(vorcstmt != nullptr) return "StmtNode: {" + vorcstmt->toString() + "}";
    else if(blockstmt != nullptr) return "StmtNode: {" + blockstmt->toString() + "}";
    else if(spexprstmt != nullptr) return "StmtNode: {" + spexprstmt->toString() + "}";
    else if(whilestmt != nullptr) return "StmtNode: {" + whilestmt->toString() + "}";
    else if(ifstmt != nullptr) return "StmtNode: {" + ifstmt->toString() + "}";
    else if(elifstmt != nullptr) return "StmtNode: {" + elifstmt->toString() + "}";
    else if(elsestmt != nullptr) return "StmtNode: {" + elifstmt->toString() + "}";
    else if(repeatstmt != nullptr) return "StmtNode: {" + repeatstmt->toString() + "}";
    else if(delstmt != nullptr) return "StmtNode: {" + delstmt->toString() + "}";
    else if(forstmt != nullptr) return "StmtNode: {" + forstmt->toString() + "}";
    else if(fdefstmt != nullptr) return "StmtNode: {" + fdefstmt->toString() + "}";
    else if(breakstmt != nullptr) return "StmtNode: {" + breakstmt->toString() + "}";
    else if(deferstmt != nullptr) return "StmtNode: {" + deferstmt->toString() + "}";
    else if(retstmt != nullptr) return "StmtNode: {" + retstmt->toString() + "}";
    else if(sdefstmt != nullptr) return "StmtNode: {" + sdefstmt->toString() + "}";
    return "null";
}

std::string AST::StructDefineStmtNode::toString() {
    std::string ret = "StructDefineStmtNode: {" + name->toString() + ", [";
    for(int i = 0; i < members.size(); i++) {
        ret += members[i]->toString();
        ret += ", ";
    }
    ret += "]}";
    return ret;
}

std::string AST::StructDefineStmtNode::memberPair::toString() {
    return "memberPair: {" + name->toString() + ", " + type->toString() + "}";
}

std::string AST::NewExprNode::toString() {
    return "NewExprNode: {" + iden->toString() + ", " + initlist->toString() + "}";
}

std::string AST::StructExprNode::toString() {
    std::string ret = "StructExprNode: {[";
    for(int i = 0; i < elements.size(); i++) {
        ret += elements[i]->toString();
        ret += ", ";
    }
    ret += "]}";
    return ret;
}

std::string AST::IdentifierNode::toString() {
    return "IdentifierNode: {" + iden->toString() + ", " + idx->toString() + "}";
}

std::string AST::CellIdentifierNode::toString() {
    return "CellIdentifierNode: {" + iden->toString() + ", " + call->toString() + "}";
}
