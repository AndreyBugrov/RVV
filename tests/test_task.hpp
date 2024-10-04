#pragma once

#include <sstream>  // stringstream

#include "../tasks/base_task.hpp"  // inheritance
#include "../common/exception.hpp"  // Exception
#include "assert.hpp"  // AssertionResult


struct TestFunctionInput{
public:
    double min_value;
    double max_value;
    size_t min_length;
    size_t max_length;
    TestFunctionInput(double input_min_value, double input_max_value, size_t input_min_length, size_t input_max_length):
    min_value(input_min_value), max_value(input_max_value), min_length(input_min_length), max_length(input_max_length)
    {}
};


class TestOutput: public BaseTaskOutput{
protected:
    bool passed_;
    
public:
    TestOutput(bool ended, std::string error_type, std::string error_msg, double seconds, bool passed): BaseTaskOutput(ended, error_type, error_msg, seconds), passed_(passed){}
    bool passed() const{return passed_;}
};

class TestTask: public BaseTask<TestFunctionInput, AssertionResult, TestOutput>{
public:
    TestTask(std::string name, std::function<AssertionResult(TestFunctionInput)> task=dumb_task<TestFunctionInput, AssertionResult>): BaseTask(name, task){}
    TestOutput run(TestFunctionInput input) const{
        bool ended = false;
        std::string error_type;
        std::string error_message;
        double seconds = 0.0;
        AssertionResult passed = false;
        try{
            const auto start_test{std::chrono::steady_clock::now()};
            passed = task_(input);
            const auto end_test{std::chrono::steady_clock::now()};
            ended = true;
            std::chrono::duration<double> test_seconds = end_test - start_test;
            if(!passed){
                error_type = "";
                error_message = passed.error_message();
            }
            seconds = test_seconds.count();
        }
        catch(Exception my_error){
            error_type = my_error.what();
            error_message = my_error.message();
        }
        catch(std::exception ex){
            error_type = ex.what();
            error_message = "Unknown";
        }
        catch(...){
            error_type = "Unknown";
            error_message = "Unknown";
        }
        return TestOutput(ended, error_type, error_message, seconds, passed);
    }
};
