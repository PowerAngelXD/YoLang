#include <iostream>
#include "offical_ysh/ysh.h"

int main(int argc, char *argv[]){
    ysh_offical::YolangShellOffical ysh;
    try{
        if(argc == 1) yvm::start(&ysh);
        else{
            if(argc > 1 && argc <= 2) {
                std::string arg1 = std::string(argv[1]);
                if(arg1 == "--version" || arg1 == "-v") {

                }
                else if(arg1 == "--help" || arg1 == "-h") {
                    std::cout<<"Yolang parameter help"<<std::endl;
                    std::cout<<"--version | -v ................. Get the current version of Yolang"<<std::endl;
                    std::cout<<"--help | -h .................... Get help with command line arguments"<<std::endl;
                    std::cout<<"--run <file> ................... Run the yolang file named 'file'"<<std::endl;
                }
            }
            else if(argc > 2 && argc <= 3) {
                std::string arg1 = std::string(argv[1]);
                std::string arg2 = std::string(argv[2]);
                // std::cout<<arg2<<std::endl;
                if(arg1 == "--run") {

                }
            }
        }
    }
    catch(yoexception::YoError e){
        std::cout<<e.what()<<std::endl;
    }
}
