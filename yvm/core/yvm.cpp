#include "yvm.h"

// native
//bif
ysto::Value vmcore::native::BuiltInFunctionSet::println(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if(args.size() != 1)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    auto content = args[0];
    switch (content.getBasicType()){
        case ytype::integer:
            std::cout << std::to_string(content.getIntegerValue().get()) << std::endl;
            break;
        case ytype::boolean:
            if (content.getBooleanValue().get()) {
                std::cout << "true" << std::endl;
            }else {
                std::cout << "false" << std::endl;
            }
            break;
        case ytype::decimal:
            std::cout << std::to_string(content.getDecimalValue().get()) << std::endl;
            break;
        case ytype::string:
            std::cout << content.getStringValue().get() << std::endl;
            break;
        case ytype::null:
            std::cout << "<null>" << std::endl;
            break;
        default:
            throw yoexception::YoError("TypeError", "Invalid Content Type", content.line, content.column);
            break;
    }
    return native::null_value; // 默认返回null
}

ysto::Value vmcore::native::BuiltInFunctionSet::input(std::vector<ysto::Value> args, ygen::byteCode code){
    std::string promptText;
    std::string resultValue;
    if(args.empty()) promptText = "";
    else {
        auto prompt = args[0];
        if (prompt.getBasicType() == ytype::basicType::string){
            promptText = prompt.getStringValue().get();
        }
    }
    std::cout << promptText;
    std::getline(std::cin, resultValue);
    return ysto::Value(ytype::YString(resultValue), false, code.line, code.column);
}

ysto::Value vmcore::native::BuiltInFunctionSet::fread(std::vector<ysto::Value> args, ygen::byteCode code) {
    if (args.size() != 1){
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    }
    auto filename = args[0];
    if (filename.getBasicType() != ytype::basicType::string){
        throw yoexception::YoError("TypeError", "Invalid FileName Type", filename.line, filename.column);
    }

    std::ifstream file(filename.getStringValue().get());
    // 编码操作
    //
    std::istreambuf_iterator<char> begin(file);
    std::istreambuf_iterator<char> end;
    std::string content(begin, end);
    return ysto::Value(ytype::YString(content), false, filename.line, filename.column);
}

ysto::Value vmcore::native::BuiltInFunctionSet::fwrite(std::vector<ysto::Value> args, ygen::byteCode code) {
    if (args.size() != 3)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    if(args[0].getBasicType() != ytype::basicType::string &&
        args[1].getBasicType() != ytype::basicType::string &&
        args[2].getBasicType() != ytype::basicType::string)
        throw yoexception::YoError("TypeError", "The parameter type filled in the corresponding function does not match the definition", args[0].line, args[0].column);
    std::string path = args[0].getStringValue().get();
    std::string content = args[1].getStringValue().get();
    std::string mode = args[2].getStringValue().get();
    /* mode是文件打开方式，分为以下几个方式
     * append：追加在已存在的文件后面，如果不存在则报错
     * new：创建一个文件，当已经有同名文件存在时报错
     * overwrite：打开一个已经存在的文件并将其里面的内容覆写，如果不存在该文件则报错
    */
    if(mode == "append") {
        std::ifstream checker(path);
        if(!checker.is_open())
            throw yoexception::YoError("FileError", "No file with path: '" + path + "' exists", code.line, code.column);
        std::ofstream file(path, std::ios::app);
        file << content;
        file.close(); file.flush();
    }
    else if(mode == "new") {
        std::ifstream checker(path);
        if(checker.is_open())
            throw yoexception::YoError("FileError", "There is already a file that exists in path: " + path, code.line, code.column);
        std::ofstream file(path);
        file << content;
        file.close(); file.flush();
    }
    else if(mode == "overwrite") {
        std::ifstream checker(path);
        if(checker.is_open())
            throw yoexception::YoError("FileError", "There is already a file that exists in path: " + path, code.line, code.column);
        std::ofstream file(path, std::ios::trunc);
        file << content;
        file.close(); file.flush();
    }
    else throw yoexception::YoError("FileError", "A schema named: '" + mode + "' does not exist", code.line, code.column);
    return native::null_value;
}

ysto::Value vmcore::native::BuiltInFunctionSet::substr(std::vector<ysto::Value> args, ygen::byteCode code){
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    else if (args.size() != 3)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);

    auto string = args[0];
    auto start = args[1];
    auto end = args[2];
    if (string.getBasicType() != ytype::basicType::string){
        throw yoexception::YoError("TypeError", "Invalid String Argument Type", string.line, string.column);
    }
    if (start.getBasicType() != ytype::basicType::integer){
        throw yoexception::YoError("TypeError", "Invalid Start Argument Type", start.line, start.column);
    }
    if (end.getBasicType() != ytype::basicType::integer){
        throw yoexception::YoError("TypeError", "Invalid End Argument Type", end.line, end.column);
    }
    std::string result = string.getStringValue().get().substr(start.getIntegerValue().get(),end.getIntegerValue().get());
    return ysto::Value(ytype::YString(result), false, string.line, string.column);
}

//ysto::Value vmcore::native::BuiltInFunctionSet::system(std::vector<ysto::Value> args, ygen::byteCode code) {
//    if(args.empty())
//        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
//    else if (args.size() != 1)
//        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
//    else if (args[0].getBasicType() != ytype::basicType::string)
//        throw yoexception::YoError("TypeError", "Invalid Command Argument Type", args[0].line, args[0].column);
//    ::system(args[0].getStringValue().get().c_str());
//    return native::null_value;
//}

//

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
    mainQueue = cs;
    constPool = cp;
}

void vmcore::vm::load(std::vector<std::string> cp, std::vector<ygen::byteCode> cs) {
    mainQueue = cs;
    constPool = cp;
}

void vmcore::vm::run(int queue_id, std::string arg) {
    auto& queue = queue_id == -1? mainQueue: codeQueue[queue_id];
    for(int i = 0; i < queue.size(); i ++) {
        auto& code  = queue[i];
        switch (code.code) {
            case ygen::tcast: tcast(code); break;
            case ygen::del_val: del_val(); break;
            case ygen::add: add(code); break;
            case ygen::push: push(code); break;
            case ygen::jmp: i = jmp(code, queue, i); break;
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
            case ygen::paraend: paraend(code); break;
            case ygen::scopestart: scopestart(code); break;
            case ygen::scopeend: scopeend(code); break;
            case ygen::idenend: idenend(code); break;
            case ygen::out: out(code); break;
            case ygen::create: create(code, i); break;
            case ygen::assign: assign(code); break;
            case ygen::del: del(code); break;
            case ygen::call: call(code); break;
        }
    }
}

ysto::Value vmcore::vm::getResult() {
    return valueStack.pop();
}

void vmcore::vm::push(ygen::byteCode code) {
    switch (code.type.bt) {
        case ytype::basicType::integer: {
            valueStack.push(ysto::Value(ytype::YInteger(code.arg1), false, code.line, code.column));
            break;
        }
        case ytype::basicType::decimal: {
            valueStack.push(ysto::Value(ytype::YDecimal(code.arg1), false, code.line, code.column));
            break;
        }
        case ytype::basicType::string: {
            valueStack.push(ysto::Value(ytype::YString(constPool[code.arg1]), false, code.line, code.column));
            break;
        }
        case ytype::basicType::boolean: {
            valueStack.push(ysto::Value(ytype::YBoolean(code.arg1), false, code.line, code.column));
            break;
        }
        case ytype::basicType::null: {
            valueStack.push(ysto::Value(code.line, code.column));
            break;
        }
        case ytype::basicType::iden: {
            std::string name = constPool[code.arg1];

            if(!space.findValue(name))
                throw yoexception::YoError("NameError", "Unknown identifier: '" + name + "'", code.line, code.column);

            if(space.getValue(name).isListValue()) {
                valueStack.push(ysto::Value(space.getValue(name).getList(), false, code.line, code.column));
            }
            else {
                switch (space.getValue(name).getBasicType()) {
                    case ytype::basicType::integer: {
                        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getIntegerValue()), false, code.line, code.column));
                        break;
                    }
                    case ytype::basicType::decimal: {
                        valueStack.push(ysto::Value(ytype::YDecimal(space.getValue(name).getDecimalValue()), false, code.line, code.column));
                        break;
                    }
                    case ytype::basicType::string: {
                        valueStack.push(ysto::Value(ytype::YString(space.getValue(name).getStringValue()), false, code.line, code.column));
                        break;
                    }
                    case ytype::basicType::boolean: {
                        valueStack.push(ysto::Value(ytype::YBoolean(space.getValue(name).getBooleanValue().get()), false, code.line, code.column));
                        break;
                    }
                    case ytype::basicType::null: {
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
    switch (left.getBasicType()) {
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
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() + right.getIntegerValue().get()),false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() + right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() + right.getIntegerValue().get()),true, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() + right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::string: {
            switch (right.getBasicType()) {
                case ytype::basicType::string: {
                    valueStack.push(ysto::Value(ytype::YString(left.getStringValue().get() + right.getStringValue().get()),false, code.line, code.column));
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
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() - right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() - right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() - right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() - right.getDecimalValue().get()), false, code.line, code.column));
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
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() * right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() * right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::string: {
                    std::string ret;
                    for(int i = 0; i < left.getIntegerValue().get(); i ++) {
                        ret += right.getStringValue().get();
                    }
                    valueStack.push(ysto::Value(ytype::YString(ret), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() * right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() * right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::string: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    std::string ret;
                    for(int i = 0; i < right.getIntegerValue().get(); i ++) {
                        ret += left.getStringValue().get();
                    }
                    valueStack.push(ysto::Value(ytype::YString(ret), false, code.line, code.column));
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
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    std::ostringstream oss;
                    oss<<(float)left.getIntegerValue().get() / right.getIntegerValue().get();
                    if(oss.str().find('.') == oss.str().npos) {
                        // 整数
                        valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() / right.getIntegerValue().get()), false, code.line, code.column));
                    }
                    else {
                        // 小数
                        valueStack.push(ysto::Value(ytype::YDecimal((float)left.getIntegerValue().get() / right.getIntegerValue().get()), false, code.line, code.column));
                    }
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getIntegerValue().get() / right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() / right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YDecimal(left.getDecimalValue().get() / right.getDecimalValue().get()), false, code.line, code.column));
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
    if(left.getBasicType() == ytype::basicType::integer) {
        if(right.getBasicType() == ytype::basicType::integer) {
            valueStack.push(ysto::Value(ytype::YInteger(left.getIntegerValue().get() % right.getIntegerValue().get()), false, code.line, code.column));
        }
        else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
    }
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::stf(ygen::byteCode code) {
    std::string name = constPool[code.arg1];
    if(name == "typeof") {
        auto value = valueStack.pop();
        switch (value.getBasicType()) {
            case ytype::integer:
                valueStack.push(ysto::Value(ytype::YString("Integer"), false, code.line, code.column));
                break;
            case ytype::boolean:
                valueStack.push(ysto::Value(ytype::YString("Boolean"), false, code.line, code.column));
                break;
            case ytype::decimal:
                valueStack.push(ysto::Value(ytype::YString("Decimal"), false, code.line, code.column));
                break;
            case ytype::string:
                valueStack.push(ysto::Value(ytype::YString("String"), false, code.line, code.column));
                break;
            case ytype::null:
                valueStack.push(ysto::Value(ytype::YString("Null"), false, code.line, code.column));
                break;
            case ytype::object:
                valueStack.push(ysto::Value(ytype::YString("Object"), false, code.line, code.column));
                break;
        }
    }
}

void vmcore::vm::lt(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() < right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() < right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() < right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() < right.getDecimalValue().get()), false, code.line, code.column));
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
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() > right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() > right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() > right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() > right.getDecimalValue().get()), false, code.line, code.column));
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
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() <= right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() <= right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() <= right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() <= right.getDecimalValue().get()), false, code.line, code.column));
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
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() >= right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() >= right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() >= right.getIntegerValue().get()), false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() >= right.getDecimalValue().get()), false, code.line, code.column));
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
    if(value.getBasicType() == ytype::basicType::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(!value.getBooleanValue().get()), false, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::logicAnd(ygen::byteCode code) {
    auto left = valueStack.pop();
    auto right = valueStack.pop();
    if(left.getBasicType() == ytype::basicType::boolean && right.getBasicType() == ytype::basicType::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(left.getBooleanValue().get() && right.getBooleanValue().get()), false, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::logicOr(ygen::byteCode code) {
    auto left = valueStack.pop();
    auto right = valueStack.pop();
    if(left.getBasicType() == ytype::basicType::boolean && right.getBasicType() == ytype::basicType::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(left.getBooleanValue().get() || right.getBooleanValue().get()), false, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}

void vmcore::vm::equ(ygen::byteCode code) {
    auto right = valueStack.pop();
    auto left = valueStack.pop();
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() == right.getIntegerValue().get()),false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() == right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() == right.getIntegerValue().get()),false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() == right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::string: {
            switch (right.getBasicType()) {
                case ytype::basicType::string: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getStringValue().get() == right.getStringValue().get()),false, code.line, code.column));
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
    switch (left.getBasicType()) {
        case ytype::basicType::integer: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() != right.getIntegerValue().get()),false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getIntegerValue().get() != right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::decimal: {
            switch (right.getBasicType()) {
                case ytype::basicType::integer: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() != right.getIntegerValue().get()),false, code.line, code.column));
                    break;
                }
                case ytype::basicType::decimal: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getDecimalValue().get() != right.getDecimalValue().get()), false, code.line, code.column));
                    break;
                }
                default: throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column); break;
            }
            break;
        }
        case ytype::basicType::string: {
            switch (right.getBasicType()) {
                case ytype::basicType::string: {
                    valueStack.push(ysto::Value(ytype::YBoolean(left.getStringValue().get() != right.getStringValue().get()),false, code.line, code.column));
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

void vmcore::vm::paraend(ygen::byteCode code) {
    valueStack.push(ysto::Value("flag:para_end"));
}

void vmcore::vm::lst(ygen::byteCode code) {
    std::vector<ysto::Value> list;
    while(valueStack.peek().getBasicType() != ytype::basicType::flag && valueStack.peek().getStringValue().get() != "flag:list_end") {
        list.push_back(valueStack.pop());
    }
    valueStack.pop(); // flag抛出去
    std::reverse(list.begin(), list.end());
    valueStack.push(ysto::Value(list, false, code.line, code.column));
}

void vmcore::vm::out(ygen::byteCode code) {
    auto result = valueStack.pop();
    if(result.isList) {
        std::cout<<"[";
        for(int i = 0; i < result.getList().size(); i ++) {
            auto elt = result.getList()[i];
            if(elt.getBasicType() == ytype::basicType::integer)
                std::cout<<elt.getIntegerValue().get();
            else if(elt.getBasicType() == ytype::basicType::decimal)
                std::cout<<elt.getDecimalValue().get();
            else if(elt.getBasicType() == ytype::basicType::string)
                std::cout<<elt.getStringValue().get();
            else if(elt.getBasicType() == ytype::basicType::boolean) {
                if(elt.getBooleanValue().get())
                    std::cout<<"true";
                else
                    std::cout<<"false";
            }
            else if(elt.getBasicType() == ytype::basicType::null) {
                std::cout<<"<null>";
            }

            if(i != result.getList().size() - 1) {
                std::cout<<", ";
            }
        }
        std::cout<<"]";
    }
    else{
        if(result.getBasicType() == ytype::basicType::integer)
            std::cout<<result.getIntegerValue().get()<<std::endl;
        else if(result.getBasicType() == ytype::basicType::decimal)
            std::cout<<result.getDecimalValue().get()<<std::endl;
        else if(result.getBasicType() == ytype::basicType::string)
            std::cout<<result.getStringValue().get()<<std::endl;
        else if(result.getBasicType() == ytype::basicType::boolean) {
            if(result.getBooleanValue().get())
                std::cout<<"true"<<std::endl;
            else
                std::cout<<"false"<<std::endl;
        }
        else if(result.getBasicType() == ytype::basicType::null) {
            std::cout<<"<null>"<<std::endl;
        }
    }
}

void vmcore::vm::create(ygen::byteCode code,  int &current) {
    std::string name = constPool[code.arg1];
    std::string state = constPool[code.arg2]; // 初始化的类型，变量还是常量；或者是Dynamic还是Static

    // 如果重命名则报错
    if(space.findValue(name))
        throw yoexception::YoError("NameError", "Identifier:'" + name + "' already exists", code.line, code.column);

    if(state == "func") {
        // 参数, 返回值处理
        std::vector<std::pair<ytype::ytypeUnit, std::string>> formalParas;
        while(valueStack.peek().getBasicType() != ytype::basicType::flag && valueStack.peek().getStringValue().get() != "flag:para_end") {
            std::string argName = valueStack.pop().getStringValue().get();
            std::string argType = valueStack.pop().getStringValue().get();
            formalParas.push_back(std::pair<ytype::ytypeUnit, std::string>(ytype::string2Type(argType),argName));
        }
        std::reverse(formalParas.begin(), formalParas.end());
        valueStack.pop();
        ytype::ytypeUnit retType = ytype::string2Type(valueStack.pop().getStringValue().get());
        // 代码块打包
        std::vector<ytype::byteCode> codes;
        int flag = 0;
        bool first = false;
        while((flag != 0 || !first) && mainQueue[current].arg1 != ygen::paraHelper::flagt::fnEnd) {
            first = true;
            current ++;
            if(mainQueue[current].code == ygen::btc::scopestart) flag ++;
            else if(mainQueue[current].code == ygen::btc::scopeend) flag --;
            codes.push_back({mainQueue[current].code, mainQueue[current].type, mainQueue[current].arg1, mainQueue[current].arg2, mainQueue[current].arg3,
                                            mainQueue[current].arg4, mainQueue[current].line, mainQueue[current].column});
        }
        //
        space.createValue(name, ysto::Value(ytype::YObject(codes, formalParas, retType), false, code.line, code.column));
    }
    else{
        auto value = valueStack.pop();
        if(code.arg4 == 1) {
            // 强制类型声明
            std::string type = constPool[code.arg3];
            if(ytype::string2BasicType(type) == value.getBasicType()) ;
            else throw yoexception::YoError("TypeError", "The expected type does not match the type given by the actual expression", code.line, code.column);
        }
        else ;

        if(value.isListValue()) {
            space.createValue(name, ysto::Value(value.getList(), state == "var" || state == "dynamic"|| state == "static"?false:true, state == "dynamic"?true:false, code.line, code.column));
        }
        else {
            switch (value.getBasicType()) {
                case ytype::integer:
                    space.createValue(name, ysto::Value(ytype::YInteger(value.getIntegerValue().get()),
                                                        state == "var" || state == "dynamic"|| state == "static"?false:true,
                                                        state == "dynamic"?true:false,
                                                        code.line,
                                                        code.column));
                    break;
                case ytype::boolean:
                    space.createValue(name, ysto::Value(ytype::YBoolean(value.getBooleanValue().get()),
                                                        state == "var" || state == "dynamic"|| state == "static"?false:true,
                                                        state == "dynamic"?true:false,
                                                        code.line,
                                                        code.column));
                    break;
                case ytype::decimal:
                    space.createValue(name, ysto::Value(ytype::YDecimal(value.getDecimalValue().get()),
                                                        state == "var" || state == "dynamic"|| state == "static"?false:true,
                                                        state == "dynamic"?true:false,
                                                        code.line,
                                                        code.column));
                    break;
                case ytype::string:
                    space.createValue(name, ysto::Value(ytype::YString(value.getStringValue().get()),
                                                        state == "var" || state == "dynamic"|| state == "static"?false:true,
                                                        state == "dynamic"?true:false,
                                                        code.line,
                                                        code.column));
                    break;
                case ytype::null:
                    space.createValue(name, ysto::Value(state == "dynamic"?true:false,code.line, code.column));
                    break;

            }
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
    if(!(space.getValue(name).getBasicType() == ytype::basicType::integer))
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
    if(!(space.getValue(name).getBasicType() == ytype::basicType::integer))
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
    if(space.getValue(name).getBasicType()!=value.getBasicType() && !space.getValue(name).isDyn())
        throw yoexception::YoError("TypeError", "The type of assignment to static variable: '" + name + "' must be its original type", code.line, code.column);
    if(code.arg1) {
        // index assignment
        // 检查是否有类型不一致的存在
        for(auto v: space.getValue(name).getList()){
            if(value.getBasicType() != v.getBasicType())
                throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
        }
        //
        auto index = valueStack.pop().getIntegerValue().get();
        space.getValue(name).getList()[index] = value;
        space.getValue(name).getType() = value.getType();
    }
    else {
        space.getValue(name) = value;
        space.getValue(name).getType() = value.getType();
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

int vmcore::vm::jmp(ygen::byteCode code, std::vector<ygen::byteCode>& queue, int current) {
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
                            if(queue[current].code == ygen::btc::scopestart) flag ++;
                            else if(queue[current].code == ygen::btc::scopeend) flag --;
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
                            if(queue[current].code == ygen::btc::scopestart) flag --;
                            else if(queue[current].code == ygen::btc::scopeend) flag ++;
                            else ;
                        }
                        current --;
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
                            if(queue[current].code == ygen::btc::scopestart) flag ++;
                            else if(queue[current].code == ygen::btc::scopeend) flag --;
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
                            if(queue[current].code == ygen::btc::scopestart) flag --;
                            else if(queue[current].code == ygen::btc::scopeend) flag ++;
                            else ;
                        }
                        current --;
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
                        if(queue[current].code == ygen::btc::scopestart) flag ++;
                        else if(queue[current].code == ygen::btc::scopeend) flag --;
                        else ;
                    }
                    break;
                }
                case ygen::paraHelper::jmpt::outLoop: {
                    int flag = 1;
                    while(queue[current].arg1 != ygen::paraHelper::flagt::loopEnd) {
                        current ++;
                        if(queue[current].code == ygen::btc::scopestart) flag ++;
                        else if(queue[current].code == ygen::btc::scopeend) flag --;
                        else ;
                    }
                    break;
                }
                case ygen::paraHelper::jmpt::backScope: {
                    int flag = 1;
                    while(flag != 0) {
                        current --;
                        if(queue[current].code == ygen::btc::scopestart) flag --;
                        else if(queue[current].code == ygen::btc::scopeend) flag ++;
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

void vmcore::vm::call(ygen::byteCode code) {
    std::string fnName = valueStack.pop().getStringValue().get();
    auto temp = valueStack.pop(); // temp值，用于检测是paraend还是正常的flag
    // 是不是无参函数: true-不是，false-是
    bool isNopara = temp.getBasicType() == ytype::basicType::flag && temp.getStringValue().get() == "flag:para_end";
    std::vector<ysto::Value> args;
    if(!isNopara) {
        args.push_back(temp);
        while(valueStack.peek().getBasicType() != ytype::basicType::flag && temp.getStringValue().get() != "flag:para_end") {
            args.push_back(valueStack.pop());
        }
    }
    std::reverse(args.begin(), args.end());
    if(std::find(yolexer::bifList.begin(), yolexer::bifList.end(), fnName) != yolexer::bifList.end()) {
        // 是bif，进行bif的判断
        if(fnName == "println") valueStack.push(native.bifSet.println(args, code));
        else if(fnName == "input") valueStack.push(native.bifSet.input(args, code));
        else if(fnName == "fread") valueStack.push(native.bifSet.fread(args, code));
        else if(fnName == "fwrite") valueStack.push(native.bifSet.fwrite(args, code));
        else if(fnName == "substr") valueStack.push(native.bifSet.substr(args, code));
        // else if(fnName == "system") valueStack.push(native.bifSet.system(args, code));
    }
    else {
        // 用户定义的函数
        if(!space.findValue(fnName))
            throw yoexception::YoError("NameError", "There is no identifier named: '" + fnName + "'", code.line, code.column);
        auto fnTemp = space.getValue(fnName);
        if(fnTemp.getBasicType() != ytype::basicType::object)
            throw yoexception::YoError("TypeError", "A value or object of type '" + ytype::basicType2String(fnTemp.getBasicType()) + "' cannot be called as a function", code.line, code.column);

        // 参数工作
        auto formalParas = fnTemp.getObjectValue().args;
        auto actParas = args;
        space.createScope("function_calling_scope", code.line, code.column); // 创建属于这个函数的作用域
        for(int i = 0; i < formalParas.size(); i++) {
            if(formalParas[i].first != actParas[i].getType())
                throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", actParas[i].line, actParas[i].column);
            space.createValue(formalParas[i].second, actParas[i]);
        }

        // 将函数代码队列push到codeQueue中
        auto codes = fnTemp.getObjectValue().codes;
        std::vector<ygen::byteCode> cs;
        for(int i = 0; i < codes.size(); i ++){
            ygen::byteCode temp = {(ygen::btc)codes[i].code, codes[i].type, codes[i].arg1, codes[i].arg2, codes[i].arg3, codes[i].arg4, codes[i].line, codes[i].column};
            cs.push_back(temp);
        }
        codeQueue.push_back(cs);

        // 运行
        run(codeQueue.size() - 1, "in_function");
        if(fnTemp.getObjectValue().retType.bt == ytype::basicType::null)
            valueStack.push(ysto::Value(code.line, code.column));
        space.deleteScope(); // 删除给函数的作用域

        // 返回值拦截
        auto ret = valueStack.pop();
        if(ret.getType() != fnTemp.getObjectValue().retType)
            throw yoexception::YoError("FunctionCallingError", "The return value is inconsistent with the design return value", code.line, code.column);
        else valueStack.push(ret);
    }
}
