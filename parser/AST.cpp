#include "AST.h"

std::string AST::FuncCallNode::toString() {
    std::string ret = "FuncCallNode: {";
    if(dots.empty()) return "FuncCallNode: {" + iden->toString() + (paras.empty()?"}":paras[0]->toString() + "}");
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

std::string AST::IdentifierNode::toString() {
    if(dots.empty()) return "IdentifierNode: {" + idens[0]->toString() + "}";
    else{
        std::string ret = "IdentifierNode: { [";
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
        else if(character != nullptr) ret += character->toString() + ", ";
        else if(iden != nullptr) ret += iden->toString() + ", ";
        else if(fcall != nullptr) ret += fcall->toString() + ", ";
        else if(siad != nullptr) ret += siad->toString() + ", ";
        else if(stf != nullptr) ret += stf->toString() + ", ";
        else if(boolconst != nullptr) ret += boolconst->toString() + ", ";

        if(op != nullptr) ret += op->toString() + "}";
        else ret += "}";
        return ret;
    }
}

std::string AST::WholeExprNode::toString() {
    if(addexpr != nullptr) return "WholeExprNode: {" + addexpr->toString() + "}";
    else if(boolexpr != nullptr) return "WholeExprNode: {" + boolexpr->toString() + "}";
    else if(listexpr != nullptr) return "WholeExprNode: {" + listexpr->toString() + "}";
    else if(assign != nullptr) return "WholeExprNode: {" + assign->toString() + "}";
    else return "null";
}

std::string AST::AssignmentExprNode::toString() {
    return "AssignmentExprNode: {" + iden->toString() + ", " + expr->toString() + (idx != nullptr? ", " + idx->toString():"") + "}";
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

// STMT

std::string AST::OutStmtNode::toString() {
    return "OutStmtNode: {" + mark->toString() + ", " + expr->toString() + "}";
}

std::string AST::VorcStmtNode::toString() {
    return "VorcStmtNode: {" + mark->toString() + ", " + 
            name->toString() + ", " + 
            (type != nullptr?type->toString():"") + ", " + 
            expr->toString() + "}";
}

std::string AST::BlockStmtNode::toString() {
    std::string ret = "BlockStmt: {[";
    for(auto stmt: stmts) {
        ret += stmt->toString();
    }
    ret += "]}";
    return ret;
}

std::string AST::SpExprStmtNode::toString() {
    if(siad != nullptr) return "SpExprStmtNode: {" + siad->toString() + "}";
    else if(assign != nullptr) return "SpExprStmtNode: {" + assign->toString() + "}";
    else if(fcall != nullptr) return "SpExprStmtNode: {" + fcall->toString() + "}";
    else return "null";
}

std::string AST::WhileStmtNode::toString() {
    return "WhileStmt: {" + cond->toString() + ", " + body->toString() + "}";
}

std::string AST::IfStmtNode::toString() {
    return "IfStmt: {" + cond->toString() + ", " + body->toString() + "}";
}

std::string AST::ElifStmtNode::toString() {
    return "ElifStmt: {" + cond->toString() + ", " + body->toString() + "}";
}

std::string AST::ElseStmtNode::toString() {
    return "ElseStmt: {" + body->toString() + "}";
}

std::string AST::DeleteStmtNode::toString() {
    return "DeleteStmt: {" + iden->toString() + "}";
}

std::string AST::RepeatStmtNode::toString() {
    return "RepeatStmt: {" + times->toString() + ", " + body->toString() + "}";
}

std::string AST::ForStmtNode::toString() {
    return "ForStmt: {" + (hasVorc?vorc->toString() + ", ":"") + (hasCond?cond->toString() + ", ":"") + (hasOp?(siad != nullptr?siad->toString():assign->toString() + "}"):"}");
}

std::string AST::FuncDefStmtNode::toString() {
    std::string ret = "FuncDef: {" + name->toString() + ", ";
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

std::string AST::BreakStmtNode::toString() {
    return "BreakStmt";
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
    return "null";
}