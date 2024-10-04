#pragma once

#include <stdexcept>  // std::length_error
#include <string> // std::string, std::to_string in error messages

// class BaseError: public std::exception{
// protected:
//     std::string msg_;
// public:
//     BaseError(std::string message): std::exception(), msg_(message){}
//     const char* what() const noexcept override{
//         return msg_.c_str();     // std::string to const char*
//     }
// };

// class AssertionError: public BaseError{
// public:
//     AssertionError(std::string message): BaseError(message){}
// };

// class TimeLimitError: public BaseError{
// public:
//     TimeLimitError(std::string message): BaseError(message){}
// };

enum class ErrorType{
    kUnknownError = 0,
    kTimeLimitError = 1,
};

class Exception{
protected:
    std::string error_type_;
    std::string error_message_;
    static std::string get_eror_type(ErrorType enum_type){
        switch (enum_type)
        {
        case ErrorType::kTimeLimitError:
            return "TimeLimitError";
            break;
        default:
            return "UnknownError";
        }
    }
public:
    Exception(ErrorType error_type, std::string error_message): error_type_(get_eror_type(error_type)), error_message_(error_message){}
    ~Exception() = default;
    std::string what() const noexcept{
        return error_type_;
    }
    std::string message() const noexcept{
        return error_message_;
    }
};
