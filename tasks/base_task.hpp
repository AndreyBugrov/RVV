#pragma once

#include <string>  // task name
#include <functional>  // task function
#include <chrono>  // time measurement

class BaseTaskOutput{
protected:
    std::string error_msg_;
    double seconds_;
    BaseTaskOutput(): error_msg_(""), seconds_(0.0){}
public:
    BaseTaskOutput(std::string error_msg, double seconds): seconds_(seconds), error_msg_(error_msg){}
    std::string what() const{ return error_msg_;}
    double time() const {return seconds_;}
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
        std::string error_msg;
        double seconds = 0.0;
        try{
            ///////////////////////// will be better to add time limits
            const std::chrono::duration<double> start_task{std::chrono::steady_clock::now()};
            task_(input);
            const std::chrono::duration<double> end_task{std::chrono::steady_clock::now()};
            std::chrono::duration<double> task_seconds = end_task - start_task;
            seconds = task_seconds.count();
        }
        /////////////////////////////// will be better to add time limit exception catcher
        catch(std::exception ex){
            error_msg = ex.what();
        }
        return TaskOutput(error_msg, seconds);
    }
};