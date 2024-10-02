#pragma once

#include "../tasks/base_task.hpp"

class TestOutput: public TaskOutput{
protected:
    bool passed_;
    
public:
    TestOutput(bool ended, std::string error_msg, double seconds, bool passed): TaskOutput(ended, error_msg, seconds), passed_(passed){}
    bool passed() const{return passed_;}
};

class TestTask: public BaseTask<bool>{
public:
    TestOutput run(TaskInput input) const{
        bool ended = false;
        std::string error_msg;
        double seconds = 0.0;
        bool passed = false;
        try{
            ///////////////////////// will be better to add time limits
            const std::chrono::duration<double> start_test{std::chrono::steady_clock::now()};
            passed = task_(input);
            const std::chrono::duration<double> end_test{std::chrono::steady_clock::now()};
            std::chrono::duration<double> test_seconds = end_test - start_test;
            seconds = test_seconds.count();
            ended = true;
        }
        /////////////////////////////// will be better to add time limit exception catcher
        catch(std::exception ex){
            error_msg = ex.what();
        }
        return TestOutput(ended, error_msg, seconds, passed);
    }
};
