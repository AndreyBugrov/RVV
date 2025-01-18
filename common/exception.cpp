#include "exception.hpp"

std::string Exception::get_eror_type(ErrorType enum_type) noexcept{
    switch (enum_type)
    {
    case ErrorType::kUnequalLengthError:
        return "UnequalLengthError";
    case ErrorType::kValueError:
        return "ValueError";
    case ErrorType::kWrongArgumentNumber:
        return "kWrongArgumentNumber";
    default:
        return "UnknownError";
    }
}