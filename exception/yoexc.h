#include <iostream>
#include <exception>

namespace yoexception{
    class YoError: public std::exception{
        std::string message;
    public:
        YoError(std::string title, std::string content, int line, int column);
        std::string what();
    };
}