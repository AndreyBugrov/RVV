#pragma once

#include <chrono>  // time measurements
#include <string>  // error messages
#include <random>  // random generator
#include <ctime>  // get current date and time

#include "../common/exception.hpp"  // exceptions
#include "../tasks/base_task_runner.hpp"  // BaseTaskRunner class
#include "test_task.hpp"  // TestTask, TestOuput classes

class TestRunner: public BaseTaskRunner{
protected:
    static const std::string delimiter;
    std::vector<TestTask>* test_tasks_; // TaskRunner does not owns its tasks
    TestFunctionInput get_test_function_input(double min_value = -50.0, double max_value = 50.0, size_t min_length = 128, size_t max_length = 256) const;
public:
    TestRunner(std::vector<TestTask>* test_tasks=nullptr): BaseTaskRunner(), test_tasks_(test_tasks){
        if(test_tasks_ == nullptr){
            task_num_ = 0;
        }else{
            task_num_ = test_tasks_->size();
        }
    }
    void run_all(std::ostream& stream = std::cout) override;
    void run_by_name(const std::string& name, std::ostream& stream = std::cout) override;
    ~TestRunner() = default;  // do not delete test_tasks
};