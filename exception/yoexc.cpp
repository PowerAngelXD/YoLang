#include "yoexc.h"
#include <string>

yoexception::YoError::YoError(std::string title, std::string content, int line, int column) {
    std::string e = "\nAn exception is thrown when the program is running:\n{{" + 
                    title + "}}:\n    " + 
                    content + 
                    (line != -1 && column != -1?"\nline: " + std::to_string(line) + ", column: " + std::to_string(column) : " ") + "\n";
    message = e;
}

std::string yoexception::YoError::what() {
    return message;
}

//

std::string yoexception::YshStopFlag::what() {
    return message;
}
