#pragma once

#include <stdexcept>  // std::length_error
#include <string> // std::string, std::to_string in error messages
#include <utility>  // std::forward

enum class ErrorType{
    kUnknownError,
    kUnequalLengthError,
    kValueError,
};

class Exception{
protected:
    std::string error_type_;
    std::string error_message_;
    static std::string get_eror_type(ErrorType enum_type) noexcept;
public:
    Exception(ErrorType error_type, std::string error_message): error_type_(get_eror_type(error_type)), error_message_(error_message){}
    virtual ~Exception() = default;
    std::string what() const noexcept{
        return error_type_;
    }
    std::string message() const noexcept{
        return error_message_;
    }
};
