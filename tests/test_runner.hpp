#pragma once

#include <functional>  // std::function
#include <chrono>  // time measurements
#include <string>  // error messages
#include <random>  // random generator

#include "../common/exception.hpp"  // exceptions 
#include "test_input.hpp"  // TestInput class
#include "test_output.hpp"  // TestOuput class
#include "test_list.hpp"  // list of all tests

template< class T>
class Test{
public:
    std::function<bool(TestInput<T>)> test_;

protected:
    std::string test_name_;
    Test(){}

public:
    TestOutput operator()(TestInput<T> input){
        const auto start_test{std::chrono::steady_clock::now()};
        bool passed = false;
        std::string error_msg;
        try{
            passed = test_(input);
        }
        catch(std::exception ex){
            error_msg = ex.what();
        }
        const auto end_test{std::chrono::steady_clock::now()};
        std::chrono::duration<double> test_seconds = end_test - start_test;
        return TestOutput(passed, test_seconds, error_msg);
    }

    Test(std::string test_name, std::function<bool(TestInput<T>)> test = dumb_test): test_name_(test_name), test_(test){}
};

class TestRunner{
protected:
    std::vector<Test<double>> double_tests_;
public:
    TestRunner(){}
    void run_all_tests();
};