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
    for(int i = 0; i < codes.size(); i ++) {
        auto& code  = codes[i];
        switch (code.code) {
            case ygen::tcast: tcast(code); break;
            case ygen::del_val: del_val(); break;
            case ygen::add: add(code); break;
            case ygen::push: push(code); break;
            case ygen::jmp: i = jmp(code, i); break;
            case ygen::selfadd: selfadd(code); break;
            case ygen::selfsub: selfsub(code); break;
            case ygen::sub: sub(code); break;
            case ygen::div: div(code); break;
            case ygen::mul: mul(code); break;
            case ygen::tmo: mod(code); break;
            case ygen::idx: idx(code); break;
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
            case ygen::scopestart: scopestart(code); break;
            case ygen::scopeend: scopeend(code); break;
            case ygen::idenend: idenend(code); break;
            case ygen::out: out(code); break;
            case ygen::create: create(code); break;
            case ygen::assign: assign(code); break;
            case ygen::del: del(code); break;
            case ygen::call:
                break;
        }
    }
}

ysto::Value vmcore::vm::getResult() {
    return valueStack.pop();
}

void vmcore::vm::push(ygen::byteCode code) {
    switch (ytype::getType(code.arg2)) {
        case ytype::vtype::integer: {
            valueStack.push(ysto::Value(ytype::YInteger(code.arg1), true, code.line, code.column));
            break;
        }
        case ytype::vtype::decimal: {
            valueStack.push(ysto::Value(ytype::YDecimal(code.arg1), true, code.line, code.column));
            break;
        }
        case ytype::vtype::string: {
            valueStack.push(ysto::Value(ytype::YString(constPool[code.arg1]), true, code.line, code.column));
            break;
        }
        case ytype::vtype::boolean: {
            valueStack.push(ysto::Value(ytype::YBoolean(code.arg1), true, code.line, code.column));
            break;
        }
        case ytype::vtype::null: {
            valueStack.push(ysto::Value(code.line, code.column));
            break;
        }
        case ytype::vtype::iden: {
            std::string name = constPool[code.arg1];

            if(!space.findValue(name))
                throw yoexception::YoError("NameError", "Unknown identifier: '" + name + "'", code.line, code.column);

            if(space.getValue(name).isListValue()) {
                valueStack.push(ysto::Value(space.getValue(name).getList(), true, code.line, code.column));
            }
            else {
                switch (space.getValue(name).getType()) {
                    case ytype::vtype::integer: {
                        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getIntegerValue()), true, code.line, code.column));
                        break;
                    }
                    case ytype::vtype::decimal: {
                        valueStack.push(ysto::Value(ytype::YDecimal(space.getValue(name).getDecimalValue()), true, code.line, code.column));
                        break;
                    }
                    case ytype::vtype::string: {
                        valueStack.push(ysto::Value(ytype::YString(space.getValue(name).getStringValue()), true, code.line, code.column));
                        break;
                    }
                    case ytype::vtype::boolean: {
                        valueStack.push(ysto::Value(ytype::YBoolean(space.getValue(name).getBooleanValue().get()), true, code.line, code.column));
                        break;
                    }
                    case ytype::vtype::null: {
                        valueStack.push(ysto::Value(code.line, code.column));
                        break;
                    }
                }
            }
        }
    }
}

void vmcore::vm::tcast(ygen::byteCode code) {
    auto right = valueStack.pop().getStringValue().get();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ytype::integer: {
            if(right == "integer")
                valueStack.push(left);
            else if(right == "decimal")
                valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get()), false, code.line, code.column));
            else if(right == "string") {
                std::ostringstream oss;
                oss << left.getIntegerValue().get();
                valueStack.push(ysto::Value(ytype::YString(oss.str()), false, code.line, code.column));
            }
            else if(right == "boolean") {
                valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() == 0?false:true), false, code.line, code.column));
            }
            else if(right == "null")
                valueStack.push(ysto::Value(code.line, code.column));
            break;
        }
        case ytype::decimal: {
            if(right == "integer")
                valueStack.push(ysto::Value(ytype::YInteger(static_cast<int>(left.getDecimalValue().get())), false, code.line, code.column));
            else if(right == "decimal")
                valueStack.push(left);
            else if(right == "string") {
                std::ostringstream oss;
                oss << left.getDecimalValue().get();
                valueStack.push(ysto::Value(ytype::YString(oss.str()), false, code.line, code.column));
            }
            else if(right == "boolean") {
                valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() == 0?false:true), false, code.line, code.column));
            }
            else if(right == "null")
                valueStack.push(ysto::Value(code.line, code.column));
            break;
        }
        case ytype::string: {
            if(right == "integer")
                valueStack.push(ysto::Value(ytype::YInteger(atoi(left.getStringValue().get().c_str())), false, code.line, code.column));
            else if(right == "decimal")
                valueStack.push(ysto::Value(ytype::YDecimal(static_cast<float>(atof(left.getStringValue().get().c_str()))), false, code.line, code.column));
            else if(right == "string")
                valueStack.push(left);
            else if(right == "boolean") {
                valueStack.push(ysto::Value(ytype::YBoolean(left.getStringValue().get() == "false"?false:true), false, code.line, code.column));
            }
            else if(right == "null")
                valueStack.push(ysto::Value(code.line, code.column));
            break;
        }
        case ytype::boolean: {
            if(right == "integer")
                valueStack.push(ysto::Value(ytype::YInteger(static_cast<int>(left.getBooleanValue().get())), false, code.line, code.column));
            else if(right == "decimal")
                valueStack.push(left);
            else if(right == "string") {
                std::ostringstream oss;
                oss << left.getDecimalValue().get();
                valueStack.push(ysto::Value(ytype::YString(oss.str()), false, code.line, code.column));
            }
            else if(right == "boolean") {
                valueStack.push(left);
            }
            else if(right == "null")
                valueStack.push(ysto::Value(code.line, code.column));
            break;
        }
        case ytype::null: {
            if(right == "integer")
                valueStack.push(ysto::Value(ytype::YInteger(0), false, code.line, code.column));
            else if(right == "decimal")
                valueStack.push(ysto::Value(ytype::YDecimal(0), false, code.line, code.column));
            else if(right == "string") {
                valueStack.push(ysto::Value(ytype::YString("null"), false, code.line, code.column));
            }
            else if(right == "boolean") {
                valueStack.push(ysto::Value(ytype::YBoolean(false), false, code.line, code.column));
            }
            else if(right == "null")
                valueStack.push(ysto::Value(code.line, code.column));
            break;
        }
    }
}
void vmcore::vm::add(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() + right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() + right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() + right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() + right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::string: {
            switch (right.getType()) {
                case ytype::vtype::string: {
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
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() - right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() - right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() - right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
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
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() * right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() * right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::string: {
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
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() * right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() * right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::string: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
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
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
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
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() / right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() / right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
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
    if(left.getType() == ytype::vtype::integer) {
        if(right.getType() == ytype::vtype::integer) {
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
            case ytype::integer:
                valueStack.push(ysto::Value(ytype::YString("Integer"), true, code.line, code.column));
                break;
            case ytype::boolean:
                valueStack.push(ysto::Value(ytype::YString("Boolean"), true, code.line, code.column));
                break;
            case ytype::decimal:
                valueStack.push(ysto::Value(ytype::YString("Decimal"), true, code.line, code.column));
                break;
            case ytype::string:
                valueStack.push(ysto::Value(ytype::YString("String"), true, code.line, code.column));
                break;
            case ytype::null:
                valueStack.push(ysto::Value(ytype::YString("Null"), true, code.line, code.column));
                break;
            case ytype::object:
                valueStack.push(ysto::Value(ytype::YString("Object"), true, code.line, code.column));
                break;
        }
    }
}

void vmcore::vm::lt(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() < right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() < right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() < right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
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
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() > right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() > right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() > right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
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
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() <= right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() <= right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() <= right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
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
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() >= right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() >= right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() >= right.getIntegerValue().get()), true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
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
    if(value.getType() == ytype::vtype::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(!value.getBooleanValue().get()), true, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::logicAnd(ygen::byteCode code) {
    auto left = valueStack.pop();
    auto right = valueStack.pop();
    if(left.getType() == ytype::vtype::boolean && right.getType() == ytype::vtype::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(left.getBooleanValue().get() && right.getBooleanValue().get()), true, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::logicOr(ygen::byteCode code) {
    auto left = valueStack.pop();
    auto right = valueStack.pop();
    if(left.getType() == ytype::vtype::boolean && right.getType() == ytype::vtype::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(left.getBooleanValue().get() || right.getBooleanValue().get()), true, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::equ(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getType()) {
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() == right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() == right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() == right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() == right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::string: {
            switch (right.getType()) {
                case ytype::vtype::string: {
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
        case ytype::vtype::integer: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() != right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() != right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::decimal: {
            switch (right.getType()) {
                case ytype::vtype::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() != right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ytype::vtype::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() != right.getDecimalValue().get()), true, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::vtype::string: {
            switch (right.getType()) {
                case ytype::vtype::string: {
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
    while(valueStack.peek().getType() != ytype::vtype::flag && valueStack.peek().getStringValue().get() != "flag:list_end") {
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
            if(elt.getType() == ytype::vtype::integer)
                std::cout<<elt.getIntegerValue().get();
            else if(elt.getType() == ytype::vtype::decimal)
                std::cout<<elt.getDecimalValue().get();
            else if(elt.getType() == ytype::vtype::string)
                std::cout<<elt.getStringValue().get();
            else if(elt.getType() == ytype::vtype::boolean) {
                if(elt.getBooleanValue().get())
                    std::cout<<"true";
                else
                    std::cout<<"false";
            }
            else if(elt.getType() == ytype::vtype::null) {
                std::cout<<"<null>";
            }

            if(i != result.getList().size() - 1) {
                std::cout<<", ";
            }
        }
        std::cout<<"]";
    }
    else{
        if(result.getType() == ytype::vtype::integer)
            std::cout<<result.getIntegerValue().get()<<std::endl;
        else if(result.getType() == ytype::vtype::decimal)
            std::cout<<result.getDecimalValue().get()<<std::endl;
        else if(result.getType() == ytype::vtype::string)
            std::cout<<result.getStringValue().get()<<std::endl;
        else if(result.getType() == ytype::vtype::boolean) {
            if(result.getBooleanValue().get())
                std::cout<<"true"<<std::endl;
            else
                std::cout<<"false"<<std::endl;
        }
        else if(result.getType() == ytype::vtype::null) {
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
        if(ytype::string2Vtype(type) == value.getType()) ;
        else throw yoexception::YoError("TypeError", "The expected type does not match the type given by the actual expression", code.line, code.column);
    }
    else ;

    if(value.isListValue()) {
        space.createValue(name, ysto::Value(value.getList(), state == "var"?false:true, code.line, code.column));
    }
    else {
        switch (value.getType()) {

            case ytype::integer:
                space.createValue(name, ysto::Value(ytype::YInteger(value.getIntegerValue().get()),
                                                    state == "var"?false:true,
                                                    code.line,
                                                    code.column));
                break;
            case ytype::boolean:
                space.createValue(name, ysto::Value(ytype::YBoolean(value.getBooleanValue().get()),
                                                    state == "var"?false:true,
                                                    code.line,
                                                    code.column));
                break;
            case ytype::decimal:
                space.createValue(name, ysto::Value(ytype::YDecimal(value.getDecimalValue().get()),
                                                    state == "var"?false:true,
                                                    code.line,
                                                    code.column));
                break;
            case ytype::string:
                space.createValue(name, ysto::Value(ytype::YString(value.getStringValue().get()),
                                                    state == "var"?false:true,
                                                    code.line,
                                                    code.column));
                break;
            case ytype::null:
                space.createValue(name, ysto::Value(code.line, code.column));
                break;

        }
    }
}

void vmcore::vm::idx(ygen::byteCode code) {
    auto idx = valueStack.pop();
    auto value = valueStack.pop();
    if(value.isListValue()) {
        if(idx.getIntegerValue().get() > value.getList().size() - 1)
            throw yoexception::YoError("ListError", "The referenced index is out of range",code.line, code.column);
        valueStack.push(ysto::Value(value.getList()[idx.getIntegerValue().get()]));
    }
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::selfadd(ygen::byteCode code) {
    auto name = valueStack.pop().getStringValue().get();

    if(!space.findValue(name))
        throw yoexception::YoError("NameError", "Unknown identifier: '" + name + "'", code.line, code.column);
    if(!(space.getValue(name).getType() == ytype::vtype::integer))
        throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);

    if(code.arg1) {
        // 是前置运算
        space.getValue(name).getIntegerValue() = ytype::YInteger(space.getValue(name).getIntegerValue().get() + 1);
        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getIntegerValue().get()), false, code.line, code.column));
    }
    else {
        // 后置运算
        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getIntegerValue().get()), false, code.line, code.column));
        space.getValue(name).getIntegerValue() = ytype::YInteger(space.getValue(name).getIntegerValue().get() + 1);
    }
}

void vmcore::vm::selfsub(ygen::byteCode code) {
    auto name = valueStack.pop().getStringValue().get();

    if(!space.findValue(name))
        throw yoexception::YoError("NameError", "Unknown identifier: '" + name + "'", code.line, code.column);
    if(!(space.getValue(name).getType() == ytype::vtype::integer))
        throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);

    if(code.arg1) {
        // 是前置运算
        space.getValue(name).getIntegerValue() = ytype::YInteger(space.getValue(name).getIntegerValue().get() - 1);
        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getIntegerValue().get()), false, code.line, code.column));
    }
    else {
        // 后置运算
        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getIntegerValue().get()), false, code.line, code.column));
        space.getValue(name).getIntegerValue() = ytype::YInteger(space.getValue(name).getIntegerValue().get() - 1);
    }
}

void vmcore::vm::assign(ygen::byteCode code) {
    auto value = valueStack.pop();
    auto name = valueStack.pop().getStringValue().get();
    if(space.getValue(name).isConst())
        throw yoexception::YoError("AssignError", "You cannot assign value to a constant", code.line, code.column);
    if(code.arg1) {
        // index assignment
        auto index = valueStack.pop().getIntegerValue().get();
        space.getValue(name).getList()[index] = value;
    }
    else {
        space.getValue(name) = value;
    }
}

void vmcore::vm::scopestart(ygen::byteCode code) {
    space.createScope("vm_created_scope", code.line, code.column);
}

void vmcore::vm::scopeend(ygen::byteCode code) {
    space.deleteScope();
}

void vmcore::vm::del_val() {
    valueStack.pop();
}

int vmcore::vm::jmp(ygen::byteCode code, int current) {
    switch ((int)code.arg1) {
        case ygen::paraHelper::jmpt::reqTrue: {
            if(valueStack.pop().getBooleanValue().get() == true) {
                switch ((int)code.arg2) {
                    case ygen::paraHelper::jmpt::outScope: {
                        int flag = 0;
                        bool first = false;
                        while(flag != 0 || !first) {
                            first = true;
                            current ++;
                            if(codes[current].code == ygen::btc::scopestart) flag ++;
                            else if(codes[current].code == ygen::btc::scopeend) flag --;
                            else ;
                        }
                        valueStack.push(ysto::Value(ytype::YBoolean(false), true, code.line, code.column));
                        break;
                    }
                    case ygen::paraHelper::jmpt::backScope: {
                        int flag = 0;
                        bool first = false;
                        while(flag != 0 || !first) {
                            first = true;
                            current --;
                            if(codes[current].code == ygen::btc::scopestart) flag --;
                            else if(codes[current].code == ygen::btc::scopeend) flag ++;
                            else ;
                        }
                        break;
                    }
                    default: break;
                }
            }
            else valueStack.push(ysto::Value(ytype::YBoolean(true), true, code.line, code.column));
            break;
        }
        case ygen::paraHelper::jmpt::reqFalse: {
            if(valueStack.pop().getBooleanValue().get() == false) {
                switch ((int)code.arg2) {
                    case ygen::paraHelper::jmpt::outScope: {
                        int flag = 0;
                        bool first = false;
                        while(flag != 0 || !first) {
                            first = true;
                            current ++;
                            if(codes[current].code == ygen::btc::scopestart) flag ++;
                            else if(codes[current].code == ygen::btc::scopeend) flag --;
                            else ;
                        }
                        valueStack.push(ysto::Value(ytype::YBoolean(true), true, code.line, code.column));
                        break;
                    }
                    case ygen::paraHelper::jmpt::backScope: {
                        int flag = 0;
                        bool first = false;
                        while(flag != 0 || !first) {
                            first = true;
                            current --;
                            if(codes[current].code == ygen::btc::scopestart) flag --;
                            else if(codes[current].code == ygen::btc::scopeend) flag ++;
                            else ;
                        }
                        break;
                    }
                    default: break;
                }
            }
            else valueStack.push(ysto::Value(ytype::YBoolean(false), true, code.line, code.column));
            break;
        }
        case ygen::paraHelper::jmpt::unconditional: {
            switch ((int)code.arg2) {
                case ygen::paraHelper::jmpt::outScope: {
                    int flag = 1;
                    while(flag != 0) {
                        current ++;
                        if(codes[current].code == ygen::btc::scopestart) flag ++;
                        else if(codes[current].code == ygen::btc::scopeend) flag --;
                        else ;
                    }
                    break;
                }
                case ygen::paraHelper::jmpt::backScope: {
                    int flag = 1;
                    while(flag != 0) {
                        current --;
                        if(codes[current].code == ygen::btc::scopestart) flag --;
                        else if(codes[current].code == ygen::btc::scopeend) flag ++;
                        else ;
                    }
                    break;
                }
                default: break;
            }
            break;
        }
        default: break;
    }
    return current;
}

void vmcore::vm::del(ygen::byteCode code) {
    if(code.arg2 == 1.0) {
        // 判断是否启用了不传入push名称的模式
        auto name = constPool[code.arg1];
        if(space.findValue(name)) {
            space.deleteValue(name);
        }
        else
            throw yoexception::YoError("NameError", "There is no identifier named: '" + name + "'", code.line, code.column);
    }
    else {
        auto name = valueStack.pop().getStringValue().get();
        if(space.findValue(name)) {
            space.deleteValue(name);
        }
        else
            throw yoexception::YoError("NameError", "There is no identifier named: '" + name + "'", code.line, code.column);
    }
}

void vmcore::vm::idenend(ygen::byteCode code) {
    valueStack.push(ysto::Value("flag:identifier_end"));
}
