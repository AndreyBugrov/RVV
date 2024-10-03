#pragma once

#include <functional>  // std::function
#include <chrono>  // time measurements
#include <string>  // error messages
#include <random>  // random generator
#include <ctime>  // get current date and time

#include "../common/exception.hpp"  // exceptions
#include "../tasks/base_task_runner.hpp"  // BaseTaskRunner class
#include "test_task.hpp"  // TestTask, TestOuput classes

class TestRunner: public BaseTaskRunner{
protected:
    std::vector<TestTask>* test_tasks_; // TaskRunner does not owns its tasks
public:
    TestRunner(std::vector<TestTask>* test_tasks=nullptr): BaseTaskRunner(), test_tasks_(test_tasks){}
    virtual void run_all(std::ostream& stream);
    ~TestRunner() = default;  // do not delete test_tasks
};