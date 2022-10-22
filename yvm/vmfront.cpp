#include "vmfront.h"

void yvm::start(yvm::yshBase yshObj) {
    yshObj.yshStart();
    try {
        while(true) yshObj.yshUpdate();
    }
    catch(yoexception::YoError e){
        yshObj.catchYoError(e);
    }
    catch(yoexception::YshStopFlag flag){}
    catch(...){}
    yshObj.yshEnd();
}

//

yvm::vmfront &yvm::vmfront::newValue(std::string name, ysto::Value value) {
    vm.space.createValue(name, value);
    return *this;
}

yvm::vmfront &yvm::vmfront::clearVM() {
    vm = vmcore::vm();
    return *this;
}

yvm::vmfront &yvm::vmfront::resetVM(std::vector<ygen::byteCode> codes, std::vector<std::string> consts) {
    vm.load(consts, codes);
    return *this;
}

yvm::vmfront &yvm::vmfront::setupVM() {
    vm.space.createValue("Point", vm.native.bisSet.Point());
    vm.space.createValue("Application", vm.native.bisSet.Application());

    vm.space.createValue("yolang", vm.native.bisiSet.appFromApplication());

    vm.space.createValue("completeVersion", ysto::Value(ytype::YString(compeleteVersion), true, 0, 0));
    vm.space.createValue("releaseVersion", ysto::Value(ytype::YString(releaseVersion), true, 0, 0));
    vm.space.createValue("_integer", ysto::Value(ytype::YString("Integer"), true, 0, 0));
    vm.space.createValue("_decimal", ysto::Value(ytype::YString("Decimal"), true, 0, 0));
    vm.space.createValue("_string", ysto::Value(ytype::YString("String"), true, 0, 0));
    vm.space.createValue("_boolean", ysto::Value(ytype::YString("Boolean"), true, 0, 0));

    for(auto fn: yolexer::bifList) {
        vm.space.createValue(fn, ysto::Value(ytype::YObject({}, {}, {}, fn), true, 0, 0));
    }

    return *this;
}

yvm::vmfront &yvm::vmfront::runVM(std::string arg) {
    vm.run(-1, arg);
    return *this;
}

//

std::vector<yolexer::yoToken> yvm::yshBase::makeTokenGroup(std::string text) {
    yolexer::Lexer lex(text);
    lex.generate();
    return lex.getTokenGroup();
}
