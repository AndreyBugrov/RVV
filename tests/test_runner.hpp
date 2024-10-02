#pragma once

#include <functional>  // std::function
#include <chrono>  // time measurements
#include <string>  // error messages
#include <random>  // random generator

#include "../common/exception.hpp"  // exceptions
#include "../tasks/base_task_runner.hpp"  // BaseTaskRunner class
#include "test_task.hpp"  // TestTask, TestOuput classes
#include "test_set.hpp"  // list of all tests

class TestRunner: public BaseTaskRunner{
protected:
    std::vector<TestTask>* double_tests_;
public:
    TestRunner(){}
    virtual void run_all();
    virtual std::string get_report();
    ~TestRunner() = default;
};