#include "AST.h"

std::string AST::FuncCallNode::toString() {
    std::string ret = "FuncCallNode: {";
    if(dots.empty()) return "FuncCallNode: {" + iden->toString() + (paras.empty()?"}":paras[0]->toString() + "}");
    else{
        ret += paras[0]->toString() + ", ";
        for(int i = 1; i < dots.size(); i++){
            ret += paras[i]->toString();
            ret += ",";
        }
        ret.erase(ret.end()-1); // 删去最后面的逗号
        return ret;
    }
}

std::string AST::IdentifierNode::toString() {
    if(dots.empty()) return "IdentifierNode: {" + idens[0]->toString() + "}";
    else{
        std::string ret = "IdentifierNode: {[";
        ret += idens[0]->toString() + ", ";
        for(int i = 0; i < dots.size(); i++){
            ret += idens[i + 1]->toString();
            ret += ",";
        }
        ret.erase(ret.end()-1); // 删去最后面的逗号
        return ret;
    }
}

std::string AST::MulExprNode::toString() {
    if(ops.empty()) return "MulExprNode: {" + prims[0]->toString() + "}";
    else{
        std::string ret = "MulExprNode: {[";
        ret += prims[0]->toString() + ", ";
        for(int i = 0; i < ops.size(); i++){
            ret += ops[i]->toString();
            ret += ", ";
            ret += prims[i + 1]->toString();
            ret += ", ";
        }
        ret.erase(ret.end()-1); // 删去最后面的逗号
        return ret;
    }
}

std::string AST::AddExprNode::toString() {
    if(ops.empty()) return "AddExprNode: {" + muls[0]->toString() + "}";
    else{
        std::string ret = "AddExprNode: {[";
        ret += muls[0]->toString() + ", ";
        for(int i = 0; i < ops.size(); i++){
            ret += ops[i]->toString();
            ret += ", ";
            ret += muls[i + 1]->toString();
            ret += ", ";
        }
        ret.erase(ret.end()-1); // 删去最后面的逗号
        return ret;
    }
}

std::string AST::BoolExprNode::toString() {
    if(ops.empty()) return "BoolExprNode: {" + cmps[0]->toString() + "}";
    else{
        std::string ret = "BoolExprNode: {[";
        ret += cmps[0]->toString() + ", ";
        for(int i = 0; i < ops.size(); i++){
            ret += ops[i]->toString();
            ret += ", ";
            ret += cmps[i + 1]->toString();
            ret += ", ";
        }
        ret.erase(ret.end()-1); // 删去最后面的逗号
        return ret;
    }
}

std::string AST::ListExprNode::toString() {
    if(dots.empty()) return "ListExprNode: {" + elements[0]->toString() + "}";
    else{
        std::string ret = "ListExprNode: {[";
        ret += elements[0]->toString() + ", ";
        for(int i = 0; i < dots.size(); i++){
            ret += elements[i + 1]->toString();
            ret += ",";
        }
        ret.erase(ret.end()-1); // 删去最后面的逗号
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

        if(op != nullptr) ret += op->toString() + "}";
        else ret += "}";
        return ret;
    }
}

std::string AST::WholeExprNode::toString() {
    if(addexpr != nullptr) return "WholeExprNode: {" + addexpr->toString() + "}";
    else if(boolexpr != nullptr) return "WholeExprNode: {" + boolexpr->toString() + "}";
    else if(listexpr != nullptr) return "WholeExprNode: {" + listexpr->toString() + "}";
    else return "null";
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