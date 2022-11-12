#include "yvm.h"

ysto::Value vmcore::gwv(ysto::Value value) {
    if(value.getRef() == nullptr) return value;
    else {
        auto temp = value.getRef();
        if(value.getRef()->getRef() != nullptr) return *temp->getRef();
        else return *temp;
    }
}

// native
//bif
ysto::Value vmcore::native::BuiltInFunctionSet::println(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if(args.size() != 1)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    auto content = args[0];
    ysto::printValue(content, "out");
    std::cout<<std::endl;
    return native::null_value; // 默认返回null
}

ysto::Value vmcore::native::BuiltInFunctionSet::input(std::vector<ysto::Value> args, ygen::byteCode code){
    std::string promptText;
    std::string resultValue;
    if(args.empty()) promptText = "";
    else {
        auto prompt = args[0];
        if (prompt.getBasicType() == ytype::basicType::string){
            promptText = gwv(prompt).getStringValue().get();
        }
    }
    std::cout << promptText;
    std::getline(std::cin, resultValue);
    return {ytype::YString(resultValue), false, code.line, code.column};
}

ysto::Value vmcore::native::BuiltInFunctionSet::fread(std::vector<ysto::Value> args, ygen::byteCode code) {
    if (args.size() != 1){
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    }
    auto filename = gwv(args[0]);
    if (filename.getBasicType() != ytype::basicType::string){
        throw yoexception::YoError("TypeError", "Invalid FileName Type", filename.line, filename.column);
    }

    std::ifstream file(filename.getStringValue().get());
    std::istreambuf_iterator<char> begin(file);
    std::istreambuf_iterator<char> end;
    std::string content(begin, end);
    return {ytype::YString(content), false, filename.line, filename.column};
}

ysto::Value vmcore::native::BuiltInFunctionSet::fwrite(std::vector<ysto::Value> args, ygen::byteCode code) {
    if (args.size() != 3)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    if(args[0].getBasicType() != ytype::basicType::string &&
        args[1].getBasicType() != ytype::basicType::string &&
        args[2].getBasicType() != ytype::basicType::string)
        throw yoexception::YoError("TypeError", "The parameter type filled in the corresponding function does not match the definition", args[0].line, args[0].column);
    std::string path = gwv(args[0]).getStringValue().get();
    std::string content = gwv(args[1]).getStringValue().get();
    std::string mode = gwv(args[2]).getStringValue().get();
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

ysto::Value vmcore::native::BuiltInFunctionSet::length(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 1)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);

    auto value = gwv(args[0]);
    if(value.getType().bt != ytype::basicType::string && value.getCompType() != ytype::compType::list)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);

    if(value.getCompType() == ytype::compType::list)
        return {ytype::YInteger(value.getList().size()), false, code.line, code.column};
    else if(value.getBasicType() == ytype::basicType::string)
        return {ytype::YInteger(value.getStringValue().get().size()), false, code.line, code.column};
}

ysto::Value vmcore::native::BuiltInFunctionSet::add_const(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 1)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    else if(args[0].getType().bt != ytype::basicType::string)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);

    return {"flag:return_const:" + gwv(args[0]).getStringValue().get()};
}

ysto::Value vmcore::native::BuiltInFunctionSet::vmcode(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 1)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    else if(args[0].getType().bt != ytype::basicType::string)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);

    return {"flag:return_code:" + gwv(args[0]).getStringValue().get()};
}

ysto::Value vmcore::native::BuiltInFunctionSet::ref(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 1)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    auto name = args[0];

    if(name.getType().bt != ytype::basicType::string)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);

    return {"flag:return_ref:" + gwv(args[0]).getStringValue().get()};
}

ysto::Value vmcore::native::BuiltInFunctionSet::substr(std::vector<ysto::Value> args, ygen::byteCode code){
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 3)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);

    auto string = gwv(args[0]);
    auto start = gwv(args[1]);
    auto end = gwv(args[2]);
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
    return {ytype::YString(result), false, string.line, string.column};
}

ysto::Value vmcore::native::BuiltInFunctionSet::randint(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 2)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    if(args[0].getType().bt != ytype::basicType::integer || args[1].getType().bt != ytype::basicType::integer)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    auto max = gwv(args[1]).getIntegerValue().get();
    auto min = gwv(args[0]).getIntegerValue().get();
    std::random_device rd;
    std::uniform_int_distribution<unsigned> ri(min, max);
    return {ytype::YInteger(ri(rd)), false, code.line, code.column};
}

ysto::Value vmcore::native::BuiltInFunctionSet::rand_deci(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 2)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    if(args[0].getType().bt != ytype::basicType::decimal || args[1].getType().bt != ytype::basicType::decimal)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    auto max = gwv(args[1]).getDecimalValue().get();
    auto min = gwv(args[0]).getDecimalValue().get();
    std::random_device rd;
    std::uniform_real_distribution<float> ri(min, max);
    return {ytype::YDecimal(ri(rd)), false, code.line, code.column};
}

ysto::Value vmcore::native::BuiltInFunctionSet::randstr(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 1)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    if(args[0].getType().bt != ytype::basicType::integer)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    auto length = gwv(args[0]).getIntegerValue().get();

    char tmp;
    std::string buffer;

    std::random_device rd;
    std::default_random_engine random(rd());

    for (int i = 0; i < length; i++) {
        tmp = random() % 36;
        if (tmp < 10)
            tmp += '0';
        else {
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return {ytype::YString(buffer), false, code.line, code.column};
}

ysto::Value vmcore::native::BuiltInFunctionSet::split(std::vector<ysto::Value> args, ygen::byteCode code) {
    if(args.empty())
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", code.line, code.column);
    else if (args.size() != 2)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);
    if(args[0].getType().bt != ytype::basicType::string || args[1].getType().bt != ytype::basicType::string)
        throw yoexception::YoError("FunctionCallingError", "Overloaded function with no specified arguments", args[0].line, args[0].column);

    std::vector<ysto::Value> ret;
    int i = 0;
    auto source = gwv(args[0]).getStringValue().get();
    auto sp = gwv(args[1]).getStringValue().get();
    while(i < source.size()) {
        std::string content;
        for(; i < source.size(); i ++) {
            if(source[i] == sp[0]) {i++; break;}
            content.push_back(source[i]);
        }
        ret.emplace_back(ytype::YString(content), false, code.line, code.column);
    }

    return {ret, false,  code.line, code.column, false};
}


//struct
ysto::Value vmcore::native::BuiltInStructSet::Point() {
    std::vector<ytype::structMemberPair> members = {
            {"x", ytype::ytypeUnit{ytype::basicType::decimal, ytype::compType::norm}},
            {"y", ytype::ytypeUnit{ytype::basicType::decimal, ytype::compType::norm}}
    };
    return {ytype::YObject(members), true, false, 0, 0};
}

ysto::Value vmcore::native::BuiltInStructSet::Application() {
    std::vector<ytype::structMemberPair> members = {
            {"author", ytype::ytypeUnit{ytype::basicType::string, ytype::compType::norm}},
            {"date", ytype::ytypeUnit{ytype::basicType::string, ytype::compType::norm}},
            {"version", ytype::ytypeUnit{ytype::basicType::string, ytype::compType::norm}}
    };
    return {ytype::YObject(members), true, false, 0, 0};
}

//class
ysto::Value vmcore::native::NativeClassSet::Process() {
    std::vector<ytype::structMemberPair> members = {
            {"proc_name", ytype::ytypeUnit{ytype::basicType::string, ytype::compType::norm}},
            {"run", ytype::ytypeUnit{ytype::basicType::object, ytype::compType::norm}}
    };

}

//struct instance
ysto::Value vmcore::native::BuiltInStructInstanceSet::appFromApplication() {
    std::map<std::string, ysto::Value> strt = {std::pair<std::string, ysto::Value>("author", ysto::Value(ytype::YString("PowerAngelXD"), true, false, 0, 0)),
                                               std::pair<std::string, ysto::Value>("date", ysto::Value(ytype::YString("2022-10-5"), true, false, 0, 0)),
                                               std::pair<std::string, ysto::Value>("version", ysto::Value(ytype::YString("Yolang 1.5.0"), true, false, 0, 0))};
    return {strt, true, 0, 0};
}
//

template<typename Type>
Type vmcore::YStack<Type>::pop() {
    if(stack.empty()) {
        throw yoexception::YoError("VM-Runtime-Error", "Attempt to pop an empty stack", -1, -1);
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
            case ygen::point_to: point_to(code); break;
            case ygen::gmem: gmem(code); break;
            case ygen::_new: _new(code); break;
            case ygen::tcast: tcast(code); break;
            case ygen::flag: flag(code); break;
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
            case ygen::type_equ: tequ(code); break;
            case ygen::equ: equ(code); break;
            case ygen::noequ: noequ(code); break;
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
            case ygen::call: call(code, arg); break;
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
            valueStack.push(ysto::Value(&space.getValue(name), false, code.line, code.column));
        }
    }
}
void vmcore::vm::flag(ygen::byteCode code) {
    if(code.arg1 == ygen::paraHelper::flagt::strtExpr) {
        std::vector<ysto::Value> strt;
        while(valueStack.peek().getBasicType() != ytype::basicType::flag) {
            strt.push_back(valueStack.pop());
        }
        // 因为std中的reverse函数需要调用重载运算符=，这样会导致bug，于是重新写一个类似的算法
        std::vector<ysto::Value> ret;
        for(int i = strt.size()-1; i >= 0; i--) {
            ret.push_back(strt[i]);
        }
        //
        valueStack.pop();
        valueStack.push(ysto::Value(ret, false, code.line, code.column, true));
    }
}
void vmcore::vm::tcast(ygen::byteCode code) {
    auto right = valueStack.pop().getStringValue().get();
    auto left = *valueStack.pop().getRef();
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
                valueStack.push(ysto::Value(ytype::YBoolean(!(left.getIntegerValue().get() == 0)), false, code.line, code.column));
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
                valueStack.push(ysto::Value(ytype::YBoolean(!(left.getDecimalValue().get() == 0)), false, code.line, code.column));
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
                valueStack.push(ysto::Value(ytype::YString(left.getBooleanValue().get()?"true":"false"), false, code.line, code.column));
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
        auto value = gwv(valueStack.pop());
        valueStack.push(ysto::Value(ytype::YString(ytype::type2String(value.getType())), false, code.line, code.column));
    }
}
void vmcore::vm::lt(ygen::byteCode code) {
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
void vmcore::vm::tequ(ygen::byteCode code) {
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
    valueStack.push(ysto::Value(ytype::YBoolean(left.getType() == right.getType()), false, code.line, code.column));
}
void vmcore::vm::no(ygen::byteCode code) {
    auto value = gwv(valueStack.pop());
    if(value.getBasicType() == ytype::basicType::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(!value.getBooleanValue().get()), false, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}
void vmcore::vm::logicAnd(ygen::byteCode code) {
    auto left =gwv(valueStack.pop());
    auto right = gwv(valueStack.pop());
    if(left.getBasicType() == ytype::basicType::boolean && right.getBasicType() == ytype::basicType::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(left.getBooleanValue().get() && right.getBooleanValue().get()), false, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}
void vmcore::vm::logicOr(ygen::byteCode code) {
    auto left = gwv(valueStack.pop());
    auto right = gwv(valueStack.pop());
    if(left.getBasicType() == ytype::basicType::boolean && right.getBasicType() == ytype::basicType::boolean)
        valueStack.push(ysto::Value(ytype::YBoolean(left.getBooleanValue().get() || right.getBooleanValue().get()), false, code.line, code.column));
    else throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
}
void vmcore::vm::equ(ygen::byteCode code) {
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    auto right = gwv(valueStack.pop());
    auto left = gwv(valueStack.pop());
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
    valueStack.push(ysto::Value(list, false, code.line, code.column, false));
}
void vmcore::vm::out(ygen::byteCode code) {
    auto result = valueStack.pop();
    ysto::printValue(result);
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
        space.createValue(name, ysto::Value(ytype::YObject(codes, formalParas, retType, name), false, code.line, code.column));
    }
    else if(state == "struct") {
        //struct A{i:integer} struct B{a:A,o:integer} var b=new B{new A{1}, 8};
        std::vector<ytype::structMemberPair> members;
        while(valueStack.peek().getBasicType() != ytype::basicType::flag && valueStack.peek().getStringValue().get() != "flag:para_end") {
            ytype::ytypeUnit type{};
            // type
            if(std::find(yolexer::typeList.begin(), yolexer::typeList.end(), valueStack.peek().getStringValue().get()) != yolexer::typeList.end()) {
                type = ytype::string2Type(valueStack.pop().getStringValue().get());
            }
            else {
                auto name = valueStack.pop().getStringValue().get();
                if(!space.findValue(name))
                    throw yoexception::YoError("NameError", "Unknown identifier: '" + name + "'", code.line, code.column);
                if(space.getValue(name).getObjectValue().getKind() == ytype::objectKind::structable) {
                    type = {ytype::basicType::object, ytype::compType::strt};
                }
            }
            //
            std::string name = valueStack.pop().getStringValue().get();
            members.push_back(ytype::structMemberPair(name, type));
        }
        std::reverse(members.begin(), members.end()); // 因为栈的原因，逆序
        space.createValue(name, ysto::Value(ytype::YObject(members), true, false, code.line, code.column));
    }
    else{
        auto value = valueStack.pop();
        if(code.arg4 == 1) {
            // 强制类型声明
            std::string type = constPool[code.arg3];
            if(ytype::string2Type(type) == value.getType()) ;
            else throw yoexception::YoError("TypeError", "The expected type does not match the type given by the actual expression", code.line, code.column);
        }
        else ;

        if(value.isListValue()) {
            space.createValue(name, ysto::Value(value.getList(),
                                                !(state == "var" || state == "dynamic" || state == "static"),
                                                state == "dynamic", code.line, code.column, false));
        }
        else if(state == "ref") {
            if(value.getCompType() != ytype::compType::ref)
                throw yoexception::YoError("TypeError", "Cannot initialize a temporary value on a reference", code.line, code.column);
            auto v = ysto::Value(value.getRef());
            v.isConstant = value.getRef()->isConst();
            space.createValue(name, v);
        }
        else if(value.getCompType() == ytype::compType::llike_strt) {
            space.createValue(name, ysto::Value(value.getList(),
                                                !(state == "var" || state == "dynamic" || state == "static"),
                                                code.line, code.column, true));
        }
        else if(value.getCompType() == ytype::compType::strt) {
            space.createValue(name, ysto::Value(value.getStrt(),
                                                !(state == "var" || state == "dynamic" || state == "static"),
                                                code.line, code.column));
        }
        else {
            auto val = gwv(value);
            switch (value.getBasicType()) {
                case ytype::integer:
                    space.createValue(name, ysto::Value(ytype::YInteger(val.getIntegerValue().get()),
                                                        !(state == "var" || state == "dynamic" || state == "static"),
                                                        state == "dynamic",
                                                        code.line,
                                                        code.column));
                    break;
                case ytype::boolean:
                    space.createValue(name, ysto::Value(ytype::YBoolean(val.getBooleanValue().get()),
                                                        !(state == "var" || state == "dynamic" || state == "static"),
                                                        state == "dynamic",
                                                        code.line,
                                                        code.column));
                    break;
                case ytype::decimal:
                    space.createValue(name, ysto::Value(ytype::YDecimal(val.getDecimalValue().get()),
                                                        !(state == "var" || state == "dynamic" || state == "static"),
                                                        state == "dynamic",
                                                        code.line,
                                                        code.column));
                    break;
                case ytype::string:
                    space.createValue(name, ysto::Value(ytype::YString(val.getStringValue().get()),
                                                        !(state == "var" || state == "dynamic" || state == "static"),
                                                        state == "dynamic",
                                                        code.line,
                                                        code.column));
                    break;
                case ytype::null:
                    space.createValue(name, ysto::Value(state == "dynamic", code.line, code.column));
                    break;

            }
        }
    }
    space.getValue(name).refName(name);
}
void vmcore::vm::assign(ygen::byteCode code) {
    auto value = gwv(valueStack.pop());
    auto sample = valueStack.pop();
    if(sample.getRef()->getRef() != nullptr)
        *sample.getRef()->getRef() = value;
    else *sample.getRef() = value;

    valueStack.push(value);
}
void vmcore::vm::point_to(ygen::byteCode code) {
    auto refto = valueStack.pop();
    auto sample = valueStack.pop();
    if(refto.getRef() == nullptr)
        throw yoexception::YoError("RefError", "Cannot reference a non identifier", code.line, code.column);

    auto name = sample.getRef()->getName();
    space.deleteValue(name);
    auto v = ysto::Value(refto.getRef());
    v.isConstant = refto.getRef()->isConst();
    space.createValue(name, v);

    valueStack.push(native::null_value);
}
void vmcore::vm::idx(ygen::byteCode code) {
    auto idx = valueStack.pop();
    auto value = valueStack.pop();
    if(!value.isListValue() && value.getCompType() != ytype::compType::llike_strt && value.getCompType() != ytype::compType::ref)
        throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);
    if(idx.getIntegerValue().get() > value.getList().size() - 1)
        throw yoexception::YoError("ListError", "The referenced index is out of range",code.line, code.column);
    if(value.getCompType() == ytype::compType::ref)
        valueStack.push(ysto::Value(value.getRef(idx.getIntegerValue().get()), false, code.line, code.column));
    else
        valueStack.push(ysto::Value(value.getList()[idx.getIntegerValue().get()]));
}
void vmcore::vm::selfadd(ygen::byteCode code) {
    auto value = valueStack.pop().getRef();

    if(value->getBasicType() != ytype::basicType::integer)
        throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);

    if(code.arg1) {
        // 是前置运算
        value->getIntegerValue().get() ++;
        valueStack.push(ysto::Value(ytype::YInteger(value->getIntegerValue().get()), false, code.line, code.column));
    }
    else {
        // 后置运算
        valueStack.push(ysto::Value(ytype::YInteger(value->getIntegerValue().get()), false, code.line, code.column));
        value->getIntegerValue().get() ++;
    }
}
void vmcore::vm::selfsub(ygen::byteCode code) {
    auto name = valueStack.pop().getStringValue().get();

    if(!space.findValue(name))
        throw yoexception::YoError("NameError", "Unknown identifier: '" + name + "'", code.line, code.column);
    if(space.getValue(name).getBasicType() != ytype::basicType::integer)
        throw yoexception::YoError("TypeError", "This operator does not support this type of operation",code.line, code.column);

    if(code.arg1) {
        // 是前置运算
        space.getValue(name).getRef()->getIntegerValue() = ytype::YInteger(space.getValue(name).getRef()->getIntegerValue().get() - 1);
        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getRef()->getIntegerValue().get()), false, code.line, code.column));
    }
    else {
        // 后置运算
        valueStack.push(ysto::Value(ytype::YInteger(space.getValue(name).getRef()->getIntegerValue().get()), false, code.line, code.column));
        space.getValue(name).getRef()->getIntegerValue() = ytype::YInteger(space.getValue(name).getRef()->getIntegerValue().get() - 1);
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
            if(valueStack.pop().getBooleanValue().get()) {
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
            if(!valueStack.pop().getBooleanValue().get()) {
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
                    bool start = false;
                    while(queue[current].arg1 != ygen::paraHelper::flagt::loopEnd || flag != 0) {
                        current --;
                        if(start && (queue[current].code == ygen::btc::scopestart||ygen::btc::scopeend)) {
                            if(queue[current].code == ygen::btc::scopestart) flag --;
                            else if(queue[current].code == ygen::btc::scopeend) flag ++;
                        }
                        else start = true;
                    }
                    flag = 0;
                    start = false;
                    while(flag != 0 || !start) {
                        start = true;
                        current++;
                        if (queue[current].code == ygen::btc::scopestart) flag++;
                        else if (queue[current].code == ygen::btc::scopeend) flag--;
                        else;
                    }

                    valueStack.push(ysto::Value(ytype::YBoolean(false), true, code.line, code.column));
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
        if(std::find(native.nativeIdenList.begin(), native.nativeIdenList.end(), name) != native.nativeIdenList.end())
            throw yoexception::YoError("NativeError", "Built in variables/constants/structures cannot be deleted", code.line, code.column);
        if(space.findValue(name)) {
            space.deleteValue(name);
        }
        else
            throw yoexception::YoError("NameError", "There is no identifier named: '" + name + "'", code.line, code.column);
    }
    else {
        auto name = valueStack.pop().getStringValue().get();
        if(std::find(native.nativeIdenList.begin(), native.nativeIdenList.end(), name) != native.nativeIdenList.end())
            throw yoexception::YoError("NativeError", "Built in variables/constants/structures cannot be deleted", code.line, code.column);
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
void vmcore::vm::call(ygen::byteCode code, std::string arg) {
    auto temp = valueStack.pop(); // temp值，用于检测是paraend还是正常的flag
    // 是不是无参函数: true-不是，false-是
    bool isNopara = temp.getBasicType() == ytype::basicType::flag && temp.getStringValue().get() == "flag:para_end";
    std::vector<ysto::Value> templ;
    std::vector<ysto::Value> args;
    if(!isNopara) {
        templ.push_back(temp);
        while(valueStack.peek().getBasicType() != ytype::basicType::flag && temp.getStringValue().get() != "flag:para_end") {
            templ.push_back(valueStack.pop());
        }
        valueStack.pop();
        for(int i = templ.size()-1; i >= 0; i--) {
            args.push_back(templ[i]);
        }
    }

    auto fnName = valueStack.pop().getRef()->getObjectValue().fnName;

    if(std::find(yolexer::bifList.begin(), yolexer::bifList.end(), fnName) != yolexer::bifList.end()) {
        // 是bif，进行bif的判断
        if(fnName == "println") valueStack.push(native.bifSet.println(args, code));
        else if(fnName == "input") valueStack.push(native.bifSet.input(args, code));
        else if(fnName == "fread") valueStack.push(native.bifSet.fread(args, code));
        else if(fnName == "fwrite") valueStack.push(native.bifSet.fwrite(args, code));
        else if(fnName == "length") valueStack.push(native.bifSet.length(args, code));
        else if(fnName == "substr") valueStack.push(native.bifSet.substr(args, code));
        else if(fnName == "randint") valueStack.push(native.bifSet.randint(args, code));
        else if(fnName == "rand_deci") valueStack.push(native.bifSet.rand_deci(args, code));
        else if(fnName == "randstr") valueStack.push(native.bifSet.randstr(args, code));
        else if(fnName == "split") valueStack.push(native.bifSet.split(args, code));
        else if(fnName == "add_const") {
            if(arg == "repl")
                throw yoexception::YoError("ReflectError", "This function cannot be used in REPL mode", code.line, code.column);
            auto value = native.bifSet.add_const(args, code).getStringValue().get();
            std::vector<std::string> ret;
            int i = 0;
            while(i < value.size()) {
                std::string content;
                for(; i < value.size(); i ++) {
                    if(value[i] == ':') {i++; break;}
                    content.push_back(value[i]);
                }
                ret.push_back(content);
            }
            constPool.push_back(ret[2]);
            valueStack.push(ysto::Value(ytype::YInteger(constPool.size()-1), false, code.line, code.column));
        }
        else if(fnName == "ref") {
            auto value = native.bifSet.ref(args, code).getStringValue().get();
            std::vector<std::string> ret;
            int i = 0;
            while(i < value.size()) {
                std::string content;
                for(; i < value.size(); i ++) {
                    if(value[i] == ':') {i++; break;}
                    content.push_back(value[i]);
                }
                ret.push_back(content);
            }
            valueStack.push(space.getValue(ret[2]));
        }
        else if(fnName == "vmcode") {
            if(arg == "repl")
                throw yoexception::YoError("ReflectError", "This function cannot be used in REPL mode", code.line, code.column);
            auto value = native.bifSet.vmcode(args, code).getStringValue().get();
            std::vector<std::string> ret;
            int i = 0;
            while(i < value.size()) {
                std::string content;
                for(; i < value.size(); i ++) {
                    if(value[i] == ':') {i++; break;}
                    content.push_back(value[i]);
                }
                ret.push_back(content);
            }
            auto codeStr = ret[2];
            std::vector<std::string> codes;
            i = 0;
            while(i < codeStr.size()) {
                std::string content;
                for(; i < codeStr.size(); i ++) {
                    if(codeStr[i] == ',') {i++; break;}
                    content.push_back(codeStr[i]);
                }
                codes.push_back(content);
            }
            if(codes.size() != 7)
                throw yoexception::YoError("VM-Runtime-Error", "It is not a valid intermediate code", code.line, code.column);
            ygen::btc bcode = ygen::string2Code(codes[0]);
            auto basicType = ytype::string2BasicType(codes[1]);
            auto compType = ytype::string2CompType(codes[2]);
            float a1=atof(codes[3].c_str()), a2=atof(codes[4].c_str()), a3=atof(codes[5].c_str()), a4=atof(codes[6].c_str());
            ygen::byteCode defcode = {bcode, {basicType, compType}, a1, a2, a3, a4, code.line, code.column};
            mainQueue.push_back(defcode);
        }
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
            if(formalParas[i].first.bt != actParas[i].getType().bt)
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
void vmcore::vm::_new(ygen::byteCode code) {
    auto name = valueStack.pop().getStringValue().get();
    if(!space.findValue(name))
        throw yoexception::YoError("NameError", "Unknown identifier: '" + name + "'", code.line, code.column);
    if(space.getValue(name).getBasicType() != ytype::basicType::object)
        throw yoexception::YoError("SyntaxError", "Non object types cannot use the new operator", code.line, code.column);

    std::vector<ysto::Value> temp;
    std::vector<ysto::Value> initlist;
    while(valueStack.peek().getBasicType() != ytype::flag) {
        temp.push_back(valueStack.pop());
    }
    valueStack.pop();
    for(int i = temp.size()-1; i >= 0; i--) {
        initlist.push_back(temp[i]);
    }

    if(space.getValue(name).getObjectValue().memberPairs.size() != initlist.size())
        throw yoexception::YoError("InitError", "The length of initialization list is different from that of sample list", code.line, code.column);

    auto sample = space.getValue(name);
    if(sample.getObjectValue().getKind() == ytype::objectKind::structable) {
        // 是struct
        std::map<std::string, ysto::Value> strt;
        for(int i = 0; i < sample.getObjectValue().memberPairs.size(); i ++) {
            if(initlist[i].getType() != sample.getObjectValue().memberPairs[i].second)
                throw yoexception::YoError("TypeError", "Different types from the sample are used for initialization", code.line, code.column);
            strt.emplace(sample.getObjectValue().memberPairs[i].first, initlist[i]);
        }
        valueStack.push(ysto::Value(strt, false, code.line, code.column));
    }
}
void vmcore::vm::gmem(ygen::byteCode code) {
    auto ref = valueStack.pop().getStringValue().get();
    auto sample = valueStack.pop();
    valueStack.push(ysto::Value(sample.getRef()->getMap(ref), false, code.line, code.column));
}
