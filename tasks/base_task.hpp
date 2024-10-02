#pragma once

#include <string>  // task name
#include <functional>  // task function
#include <chrono>  // time measurement

struct TaskInput{
public:
    double min_value_;
    double max_value_;
    size_t min_length_;
    size_t max_length_;
    TaskInput(double min_value, double max_value, size_t min_length, size_t max_length):
    min_value_(min_value), max_value_(max_value), min_length_(min_length), max_length_(max_length)
    {}
};

class TaskOutput{
protected:
    bool ended_;
    std::string error_msg_;
    double seconds_;
    TaskOutput(): ended_(false), error_msg_(""), seconds_(0.0){}
public:
    TaskOutput(bool ended, std::string error_msg, double seconds): ended_(ended), seconds_(seconds), error_msg_(error_msg){}
    bool ended() const {return ended_;}
    std::string what() const{ return error_msg_;}
    double time() const {return seconds_;}
};

template<class T>
class BaseTask{
protected:
    BaseTask() = default;
    std::string name_;
    std::function<T(TaskInput)> task_;
public:
    BaseTask(std::string name, std::function<T(TaskInput)> task): name_(name), task_(task){}
    ~BaseTask() = default;
    std::function<T(TaskInput)>& task() const {return task_}
    TaskOutput run(TaskInput input) const{
        bool ended = false;
        std::string error_msg;
        double seconds = 0.0;
        try{
            ///////////////////////// will be better to add time limits
            const std::chrono::duration<double> start_test{std::chrono::steady_clock::now()};
            task_(input);
            const std::chrono::duration<double> end_test{std::chrono::steady_clock::now()};
            std::chrono::duration<double> test_seconds = end_test - start_test;
            seconds = test_seconds.count();
            ended = true;
        }
        /////////////////////////////// will be better to add time limit exception catcher
        catch(std::exception ex){
            error_msg = ex.what();
        }
        return TaskOutput(ended, error_msg, seconds);
    }
};