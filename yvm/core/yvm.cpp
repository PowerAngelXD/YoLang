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

void vmcore::vm::load(std::vector<std::string> cp, std::vector<ygen::byteCode> cs) {
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
            case ygen::sub: sub(code); break;
            case ygen::div: div(code); break;
            case ygen::mul: mul(code); break;
            case ygen::tmo: mod(code); break;
            case ygen::idx:
                break;
            case ygen::lst: lst(code); break;
            case ygen::logicand: logicAnd(code); break;
            case ygen::logicor: logicOr(code); break;
            case ygen::no: no(code); break;
            case ygen::lt: lt(code); break;
            case ygen::ltet: ltet(code); break;
            case ygen::gt: gt(code); break;
            case ygen::gtet: gtet(code); break;
            case ygen::equ: equ(code); break;
            case ygen::noequ: noequ(code); break;
            case ygen::gmem:
                break;
            case ygen::stf: stf(code); break;
            case ygen::listend: lstend(code); break;
            case ygen::paraend:
                break;
            case ygen::scopestart:
                break;
            case ygen::scopeend:
                break;
            case ygen::idenend:
                break;
            case ygen::out: out(code); break;
            case ygen::create: create(code); break;
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
        case ygen::type::vtype::boolean: {
            valueStack.push(ysto::Value(ytype::YBoolean(code.arg1), true, code.line, code.column));
            break;
        }
        case ygen::type::vtype::null: {
            valueStack.push(ysto::Value(code.line, code.column));
            break;
        }
        case ygen::type::vtype::iden: {
            std::string name = constPool[code.arg1];
            if(space.getValue(name).isListValue()) {

            }
            else {
                switch (space.getValue(name).getType()) {
                    case ygen::type::vtype::integer: {
                        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getIntegerValue().get()), true, code.line, code.column));
                        break;
                    }
                    case ygen::type::vtype::decimal: {
                        valueStack.push(ysto::Value(ytype::YDecimal(space.getValue(name).getDecimalValue().get()), true, code.line, code.column));
                        break;
                    }
                    case ygen::type::vtype::string: {
                        valueStack.push(ysto::Value(ytype::YString(space.getValue(name).getStringValue().get()), true, code.line, code.column));
                        break;
                    }
                    case ygen::type::vtype::boolean: {
                        valueStack.push(ysto::Value(ytype::YBoolean(space.getValue(name).getBooleanValue().get()), true, code.line, code.column));
                        break;
                    }
                    case ygen::type::vtype::null: {
                        valueStack.push(ysto::Value(code.line, code.column));
                        break;
                    }
                }
            }
        }
    }
}

void vmcore::vm::add(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() + right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() + right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() + right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() + right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::string: {
            switch (right.getType()) {
                case ygen::type::vtype::string: {
                    valueStack.push(ysto::Value(ytype::YString(left.getStringValue().get() + right.getStringValue().get()),true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::sub(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() - right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() - right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() - right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() - right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::mul(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() * right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() * right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::string: {
                    std::string ret;
                    for(int i = 0; i < left.getIntegerValue().get(); i ++) {
                        ret += right.getStringValue().get();
                    }
                    valueStack.push(ysto::Value(ytype::YString(ret), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() * right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() * right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::string: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    std::string ret;
                    for(int i = 0; i < right.getIntegerValue().get(); i ++) {
                        ret += left.getStringValue().get();
                    }
                    valueStack.push(ysto::Value(ytype::YString(ret), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::div(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    std::ostringstream oss;
                    oss<<(float)left.getIntegerValue().get() / right.getIntegerValue().get();
                    if(oss.str().find('.') == oss.str().npos) {
                        // 整数
                        valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() / right.getIntegerValue().get()), true, code.line, code.column));
                    }
                    else {
                        // 小数
                        valueStack.push(ysto::Value(ytype::YDecimal((float)left.getIntegerValue().get() / right.getIntegerValue().get()), true, code.line, code.column));
                    }
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() / right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() / right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() / right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::mod(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    if(left.getType() == ygen::type::vtype::integer) {
        if(right.getType() == ygen::type::vtype::integer) {
            valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() % right.getIntegerValue().get()), true, code.line, code.column));
        }
        else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
    }
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::stf(ygen::byteCode code) {
    std::string name = constPool[code.arg1];
    if(name == "typeof") {
        auto value = valueStack.pop();
        switch (value.getType()) {
            case ygen::type::integer:
                valueStack.push(ysto::Value(ytype::YString("Integer"), true, code.line, code.column));
                break;
            case ygen::type::boolean:
                valueStack.push(ysto::Value(ytype::YString("Boolean"), true, code.line, code.column));
                break;
            case ygen::type::decimal:
                valueStack.push(ysto::Value(ytype::YString("Decimal"), true, code.line, code.column));
                break;
            case ygen::type::string:
                valueStack.push(ysto::Value(ytype::YString("String"), true, code.line, code.column));
                break;
            case ygen::type::null:
                valueStack.push(ysto::Value(ytype::YString("Null"), true, code.line, code.column));
                break;
            case ygen::type::object:
                valueStack.push(ysto::Value(ytype::YString("Object"), true, code.line, code.column));
                break;
        }
    }
}

void vmcore::vm::lt(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() < right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() < right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() < right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() < right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::gt(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() > right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() > right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() > right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() > right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::ltet(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() <= right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() <= right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() <= right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() <= right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::gtet(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() >= right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() >= right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() >= right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() >= right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::no(ygen::byteCode code) {
    auto value = valueStack.pop();
    if(value.getType() == ygen::type::vtype::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(!value.getBooleanValue().get()), true, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::logicAnd(ygen::byteCode code) {
    auto left = valueStack.pop();
    auto right = valueStack.pop();
    if(left.getType() == ygen::type::vtype::boolean && right.getType() == ygen::type::vtype::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(left.getBooleanValue().get() && right.getBooleanValue().get()), true, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::logicOr(ygen::byteCode code) {
    auto left = valueStack.pop();
    auto right = valueStack.pop();
    if(left.getType() == ygen::type::vtype::boolean && right.getType() == ygen::type::vtype::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(left.getBooleanValue().get() || right.getBooleanValue().get()), true, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::equ(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() == right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() == right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() == right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() == right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::string: {
            switch (right.getType()) {
                case ygen::type::vtype::string: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getStringValue().get() == right.getStringValue().get()),true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::noequ(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ygen::type::vtype::integer: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() != right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() != right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::decimal: {
            switch (right.getType()) {
                case ygen::type::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() != right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ygen::type::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() != right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ygen::type::vtype::string: {
            switch (right.getType()) {
                case ygen::type::vtype::string: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getStringValue().get() != right.getStringValue().get()),true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
    }
}

void vmcore::vm::lstend(ygen::byteCode code) {
     valueStack.push(ysto::Value("flag:list_end"));
}

void vmcore::vm::lst(ygen::byteCode code) {
    std::vector<ysto::Value> list;
    while(valueStack.peek().getType() != ygen::type::vtype::flag && valueStack.peek().getStringValue().get() != "flag:list_end") {
        list.push_back(valueStack.pop());
    }
    valueStack.pop(); // flag抛出去
    std::reverse(list.begin(), list.end());
    valueStack.push(ysto::Value(list, true, code.line, code.column));
}

void vmcore::vm::out(ygen::byteCode code) {
    auto result = valueStack.pop();
    if(result.isList) {
        std::cout<<"[";
        for(int i = 0; i < result.getList().size(); i ++) {
            auto elt = result.getList()[i];
            if(elt.getType() == ygen::type::vtype::integer)
                std::cout<<elt.getIntegerValue().get();
            else if(elt.getType() == ygen::type::vtype::decimal)
                std::cout<<elt.getDecimalValue().get();
            else if(elt.getType() == ygen::type::vtype::string)
                std::cout<<elt.getStringValue().get();
            else if(elt.getType() == ygen::type::vtype::boolean) {
                if(elt.getBooleanValue().get())
                    std::cout<<"true";
                else
                    std::cout<<"false";
            }
            else if(elt.getType() == ygen::type::vtype::null) {
                std::cout<<"<null>";
            }

            if(i != result.getList().size() - 1) {
                std::cout<<", ";
            }
        }
        std::cout<<"]";
    }
    else{
        if(result.getType() == ygen::type::vtype::integer)
            std::cout<<result.getIntegerValue().get()<<std::endl;
        else if(result.getType() == ygen::type::vtype::decimal)
            std::cout<<result.getDecimalValue().get()<<std::endl;
        else if(result.getType() == ygen::type::vtype::string)
            std::cout<<result.getStringValue().get()<<std::endl;
        else if(result.getType() == ygen::type::vtype::boolean) {
            if(result.getBooleanValue().get())
                std::cout<<"true"<<std::endl;
            else
                std::cout<<"false"<<std::endl;
        }
        else if(result.getType() == ygen::type::vtype::null) {
            std::cout<<"<null>"<<std::endl;
        }
    }
}

void vmcore::vm::create(ygen::byteCode code) {
    std::string name = constPool[code.arg1];
    std::string state = constPool[code.arg2]; // 初始化的类型，变量还是常量
    auto value = valueStack.pop();

    if(code.arg4 == 1) {
        // 类型声明
        std::string type = constPool[code.arg3];
        if(ygen::type::string2Vtype(type) == value.getType()) ;
        else throw yoexception::YoError("TypeError", "The expected type does not match the type given by the actual expression", code.line, code.column);
    }
    else ;

    if(value.isListValue()) {
        space.createValue(name, ysto::Value(value.getList(), state == "var"?false:true, code.line, code.column));
    }
    else {
        switch (value.getType()) {

            case ygen::type::integer:
                space.createValue(name, ysto::Value(ytype::YInteger(value.getIntegerValue().get()),
                                                    state == "var"?false:true,
                                                    code.line,
                                                    code.column));
                break;
            case ygen::type::boolean:
                space.createValue(name, ysto::Value(ytype::YBoolean(value.getBooleanValue().get()),
                                                    state == "var"?false:true,
                                                    code.line,
                                                    code.column));
                break;
            case ygen::type::decimal:
                space.createValue(name, ysto::Value(ytype::YDecimal(value.getDecimalValue().get()),
                                                    state == "var"?false:true,
                                                    code.line,
                                                    code.column));
                break;
            case ygen::type::string:
                space.createValue(name, ysto::Value(ytype::YString(value.getStringValue().get()),
                                                    state == "var"?false:true,
                                                    code.line,
                                                    code.column));
                break;
            case ygen::type::null:
                space.createValue(name, ysto::Value(code.line, code.column));
                break;

        }
    }
}
