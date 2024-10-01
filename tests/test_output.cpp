#include "test_output.hpp"

bool TestOutput::passed() const{
    return passed_;
}

std::string TestOutput::what() const{
    return error_msg_;
}
