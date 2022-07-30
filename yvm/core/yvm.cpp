#include "yvm.h"

template<typename Type>
Type vmcore::YStack<Type>::pop() {
    if(stack.empty()) {
        throw yoexception::YoError("VMError", "Attempt to pop an empty stack", -1, -1);
    }
    Type value = stack[stack.size() - 1];
    stack.erase(stack.end());
    return value;
}

template<typename Type>
Type vmcore::YStack<Type>::peek() {
    return stack[stack.size() - 1];
}

template<typename Type>
void vmcore::YStack<Type>::push(Type value) {
    stack.push_back(value);
}

// VM

vmcore::vm::vm(std::vector<ygen::byteCode> cs, std::vector<std::string> cp) {
    codes = cs;
    constPool = cp;
}

void vmcore::vm::run(std::string arg) {
    for(auto code: codes) {
        switch (code.code) {
            case ygen::add: add(code); break;
            case ygen::push: push(code); break;
            case ygen::gto:
                break;
            case ygen::jmp:
                break;
            case ygen::selfadd:
                break;
            case ygen::selfsub:
                break;
            case ygen::sub:
                break;
            case ygen::div:
                break;
            case ygen::mul:
                break;
            case ygen::tmo:
                break;
            case ygen::idx:
                break;
            case ygen::lst:
                break;
            case ygen::logicand:
                break;
            case ygen::logicor:
                break;
            case ygen::no:
                break;
            case ygen::lt:
                break;
            case ygen::ltet:
                break;
            case ygen::gt:
                break;
            case ygen::gtet:
                break;
            case ygen::equ:
                break;
            case ygen::noequ:
                break;
            case ygen::gmem:
                break;
            case ygen::stf:
                break;
            case ygen::listend:
                break;
            case ygen::paraend:
                break;
            case ygen::scopestart:
                break;
            case ygen::scopeend:
                break;
            case ygen::idenend:
                break;
            case ygen::out:
                break;
            case ygen::define:
                break;
            case ygen::init:
                break;
            case ygen::assign:
                break;
            case ygen::del:
                break;
            case ygen::call:
                break;
        }
    }
}

ysto::Value vmcore::vm::getResult() {
    return valueStack.pop();
}

void vmcore::vm::add(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    if(left.getType() == ygen::type::vtype::integer) {
        if(right.getType() == ygen::type::vtype::integer) {
            valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() + right.getIntegerValue().get()), true, code.line, code.column));
        }
        else if(right.getType() == ygen::type::vtype::decimal) {
            valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() + right.getDecimalValue().get()), true, code.line, code.column));
        }
        else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
    }
    else if(left.getType() == ygen::type::vtype::decimal) {
        if(right.getType() == ygen::type::vtype::integer) {
            valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() + right.getIntegerValue().get()), true, code.line, code.column));
        }
        else if(right.getType() == ygen::type::vtype::decimal) {
            valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() + right.getDecimalValue().get()), true, code.line, code.column));
        }
        else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
    }
    else if(left.getType() == ygen::type::vtype::string) {
        if(right.getType() == ygen::type::vtype::string) {
            valueStack.push(ysto::Value(ytype::YString(left.getStringValue().get() + right.getStringValue().get()), true, code.line, code.column));
        }
        else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
    }
}

void vmcore::vm::push(ygen::byteCode code) {
    switch (ygen::type::getType(code.arg2)) {
        case ygen::type::vtype::integer: {
            valueStack.push(ysto::Value(ytype::YInteger(code.arg1), true, code.line, code.column));
            break;
        }
        case ygen::type::vtype::decimal: {
            valueStack.push(ysto::Value(ytype::YDecimal(code.arg1), true, code.line, code.column));
            break;
        }
        case ygen::type::vtype::string: {
            valueStack.push(ysto::Value(ytype::YString(constPool[code.arg1]), true, code.line, code.column));
            break;
        }
    }
}
