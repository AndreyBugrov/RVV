#pragma once

#include <stdexcept>  // std::length_error
#include <string> // std::string, std::to_string in error messages

class AssertionError: public std::exception{
protected:
    std::string msg_;
public:
    AssertionError(std::string message): std::exception(), msg_(message){}
    const char* what() const noexcept override{
        return msg_.c_str();     // std::string to const char*
    }
};