#pragma once

#include <string>  // task name
#include <functional>  // task function
#include <chrono>  // time measurement

#include "../common/exception.hpp"  // Exception

class BaseTaskOutput{
protected:
    bool ended_;
    std::string error_type_;
    std::string error_message_;
    double seconds_;
    BaseTaskOutput(): error_message_(""), seconds_(0.0){}
public:
    BaseTaskOutput(bool ended, std::string error_type, std::string error_msg, double seconds): ended_(ended), error_type_(error_type), seconds_(seconds), error_message_(error_msg){}
    std::string what() const noexcept{ return error_type_;}
    std::string error_message() const noexcept{return error_message_;}
    double time() const noexcept{return seconds_;}
    bool ended() const noexcept{return ended_;}
};

template<class FunctionInput, class FunctionOutput> FunctionOutput dumb_task(FunctionInput input){return FunctionOutput{};}

template<class FunctionInput, class FunctionOutput, class TaskOutput>
class BaseTask{
protected:
    std::string name_;
    std::function<FunctionOutput(FunctionInput)> task_;
public:
    BaseTask() = delete;
    BaseTask(std::string name, std::function<FunctionOutput(FunctionInput)> task=dumb_task): name_(name), task_(task){}
    ~BaseTask() = default;
    std::function<FunctionOutput(FunctionInput)>& task() const {return task_;}
    std::string name() const {return name_;}
    TaskOutput run(FunctionInput input) const{
        bool ended = false;
        std::string error_type = "";
        std::string error_message = "";
        double seconds = 0.0;
        try{
            const auto start_test{std::chrono::steady_clock::now()};
            task_(input);
            const auto end_test{std::chrono::steady_clock::now()};
            ended = true;
            std::chrono::duration<double> test_seconds = end_test - start_test;
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
        return TaskOutput(ended, error_type, error_message, seconds);
    }
};