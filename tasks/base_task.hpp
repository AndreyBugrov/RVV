#pragma once

#include <string>  // task name
#include <functional>  // task function
#include <chrono>  // time measurement

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