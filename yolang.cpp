#include <iostream>
#include "console/yoshell.h"

int main(int argc, char *argv[]){
    try{
        if(argc == 1) ysh::runYoShell();
        else{
            if(argc > 1 && argc <= 2) {
                std::string arg1 = std::string(argv[1]);
                if(arg1 == "--version" || arg1 == "--v") {
                    std::cout<<"Yolang version: " << ysh::completeVersion << "(" << ysh::version << ")" << " rel:" << ysh::relVersion << std::endl;
                }
                else if(arg1 == "--help" || arg1 == "--h") {
                    std::cout<<"Yolang parameter help"<<std::endl;
                    std::cout<<"--version | --v ................. Get the current version of Yolang"<<std::endl;
                    std::cout<<"--help | --h .................... Get help with command line arguments"<<std::endl;
                    std::cout<<"--gen <file> <target> ........... Generate a Yolang bytecode file named 'Target' according to the file named 'file'"<<std::endl;
                    std::cout<<"--run <file> ...... ............. Run the yolang file named 'file'"<<std::endl;
                }
            }
            else if(argc > 2 && argc <= 3) {
                std::string arg1 = std::string(argv[1]);
                std::string arg2 = std::string(argv[2]);
                // std::cout<<arg2<<std::endl;
                if(arg1 == "--run") {
                    ysh::insRun({arg2});
                }
            }
            else if(argc > 3 && argc <= 4) {
                std::string arg1 = std::string(argv[1]);
                std::string arg2 = std::string(argv[2]);
                std::string arg3 = std::string(argv[3]);
                if(arg1 == "--gen") {
                    ysh::insGen({arg2, arg3});
                }
            }
        }
    }
    catch(yoexception::YoError e){
        std::cout<<e.what()<<std::endl;
    }
}