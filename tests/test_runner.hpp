#pragma once

#include <chrono>  // time measurements
#include <string>  // error messages
#include <random>  // random generator
#include <ctime>  // get current date and time

#include "../common/exception.hpp"  // exceptions
#include "../tasks/base_task_runner.hpp"  // BaseTaskRunner class
#include "test_task.hpp"  // TestTask, TestOuput classes

class TestRunner: public BaseTaskRunner{
private:
    static const double kMinValue_;
    static const double kMaxValue_;
    static const size_t kMinLength_;
    static const size_t kMaxLength_;

    void print_header(std::ostream& stream) const;
    TestFunctionInput create_and_store_input();
    TestFunctionInput get_test_function_input() const;
    void init_run(std::ostream& stream);
protected:
    static const std::string delimiter;
    std::vector<TestTask>* test_tasks_; // TaskRunner does not owns its tasks
    std::vector<TestFunctionInput> task_inputs_;
public:
    TestRunner(std::vector<TestTask>* test_tasks=nullptr): BaseTaskRunner(), test_tasks_(test_tasks), task_inputs_(0){
        if(test_tasks_ == nullptr){
            task_num_ = 0;
        }else{
            task_num_ = test_tasks_->size();
        }
    }
    virtual void run_all(std::ostream& stream = std::cout) override;
    virtual void run_by_name(const std::string& name, std::ostream& stream = std::cout) override;
    virtual ~TestRunner() = default;  // do not delete test_tasks
};