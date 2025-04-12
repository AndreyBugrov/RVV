#include "exception.hpp"

std::string Exception::get_eror_type(ErrorType enum_type) noexcept{
    switch (enum_type)
    {
    case ErrorType::kUnequalLengthError:
        return "UnequalLengthError";
    case ErrorType::kValueError:
        return "ValueError";
    case ErrorType::kIncorrectArgumentCount:
        return "WrongArgumentCount";
    case ErrorType::kIncorrectLengthRatio:
        return "IncorrectLengthRatio";
    case ErrorType::kUnexpectedCase:
        return "UnexpectedCase";
    default:
        return "UnknownError";
    }
}