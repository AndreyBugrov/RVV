#pragma once

#include <stdexcept>  // std::length_error
#include <string> // std::string, std::to_string in error messages
#include <sstream>  // error message generation
#include <utility>  // std::forward

enum class ErrorType{
    kUnknownError = 0,
    kUnequalLengthError = 1,
};






class Exception{
protected:
    std::string error_type_;
    std::string error_message_;
    static std::string get_eror_type(ErrorType enum_type){
        switch (enum_type)
        {
        case ErrorType::kUnequalLengthError:
            return "UnequalLengthError";
            break;
        default:
            return "UnknownError";
        }
    }

    template <typename T>
    static void MagicLog(std::ostream& o, T t)
    {
        o << t << std::endl;
    }

    template<typename T, typename... Args>
    static void MagicLog(std::ostream& o, T t, Args... args) // recursive variadic function
    {
        MagicLog(o, t);
        MagicLog(o, args...);
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

    template<typename... Args>
    static std::string create_eror_message(Args... args)
    {
        std::ostringstream oss;
        MagicLog(oss, args...);
        return oss.str();
    }
};
