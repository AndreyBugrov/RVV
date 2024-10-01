#pragma once

#include <string>  // error messages

class TestOutput{
protected:
    bool passed_;
    double seconds_;
    std::string error_msg_;
    TestOutput(): passed_(false), seconds_(0.0), error_msg_(""){}
    
public:
    TestOutput(bool passed, double seconds, std::string error_msg): passed_(passed), seconds_(seconds), error_msg_(error_msg){}
    bool passed() const;
    std::string what() const;

    // can add friend operator<<
};