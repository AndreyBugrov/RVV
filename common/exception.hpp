#pragma once

#include <stdexcept>  // std::length_error
#include <string> // std::string, std::to_string in error messages

class BaseError: public std::exception{
protected:
    std::string msg_;
public:
    const static std::string name;
    BaseError(std::string message): std::exception(), msg_(message){}
    const char* what() const noexcept override{
        return msg_.c_str();     // std::string to const char*
    }
};

class AssertionError: public BaseError{
public:
    const static std::string name;
    AssertionError(std::string message): BaseError(message){}
};

class TimeLimitError: public BaseError{
public:
    const static std::string name;
    TimeLimitError(std::string message): BaseError(message){}
};
