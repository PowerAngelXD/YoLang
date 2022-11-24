#include <iostream>
#include "offical_ysh/ysh.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
    ysh_offical::YolangShellOffical ysh;
    ysh.front.setupVM();
    try{
        if(argc == 1) yvm::start(&ysh);
        else{
            if(argc > 1 && argc <= 2) {
                std::string arg1 = std::string(argv[1]);
                if(arg1 == "--version" || arg1 == "-v") {

                }
                else if(arg1 == "--help" || arg1 == "-h") {
                    cout<<"Yolang parameter help"<<endl;
                    cout<<"--version\t-v ................. Get the current version of Yolang"<<endl;
                    cout<<"--help   \t-h ................. Get help with command line arguments"<<endl;
                    cout<<"--run <file>  ................. Run the yolang file named 'file'"<<endl;
                }
            }
            else if(argc > 2 && argc <= 3) {
                std::string arg1 = std::string(argv[1]);
                std::string arg2 = std::string(argv[2]);
                if(arg1 == "--run") {
                    auto name = arg2;
                    if(yvm::tools::compareFileType(name, ".yo")){
                        std::ifstream file(name);
                        std::istreambuf_iterator<char> begin(file);
                        std::istreambuf_iterator<char> end;
                        std::string str(begin, end);


                        auto tg = ysh.makeTokenGroup(str);
                        ysh.yparser = parser::Parser(tg);
                        auto stmts = ysh.yparser.parse();
                        ygen::ByteCodeGenerator bcg;
                        bcg.visit(stmts);
                        ysh.front.resetVM(bcg.getCodes(), bcg.getConstPool());
                        ysh.front.setupVM().runVM("program").clearVM().setupVM();
                    }
                    else
                        throw yoexception::YoError("FileError", "Not any file that can be run", -1, -1);
                }
            }
        }
    }
    catch(yoexception::YoError e){
        cout<<e.what()<<endl;
    }
}
