#pragma once

#include "../tasks/base_task.hpp"  // inheritance
#include "../common/exception.hpp"  // AssertionError


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
    TestOutput(std::string error_msg, double seconds, bool passed): BaseTaskOutput(error_msg, seconds), passed_(passed){}
    bool passed() const{return passed_;}
};

class TestTask: public BaseTask<TestFunctionInput, bool, TestOutput>{
public:
    TestTask(std::string name, std::function<bool(TestFunctionInput)> task=dumb_task<TestFunctionInput, bool>): BaseTask(name, task){}
    TestOutput run(TestFunctionInput input) const{
        bool ended = false;
        std::string error_msg;
        double seconds = 0.0;
        bool passed = false;
        try{
            ///////////////////////// will be better to add time limits
            const auto start_test{std::chrono::steady_clock::now()};
            passed = task_(input);
            const auto end_test{std::chrono::steady_clock::now()};
            std::chrono::duration<double> test_seconds = end_test - start_test;
            seconds = test_seconds.count();
            ended = true;
        }
        /////////////////////////////// will be better to add time limit exception catcher
        catch(AssertionError ass_error){
            error_msg = ass_error.what();
        }
        catch(std::exception ex){
            error_msg = ex.what();
        }
        return TestOutput(error_msg, seconds, passed);
    }
};
