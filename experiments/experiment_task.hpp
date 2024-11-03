#pragma once

#include "../tasks/base_task.hpp"  // BaseTaskOutput

template<typename Foo, typename... Args>
BaseTaskOutput run_experiment(Foo task, Args... args){
    bool ended = false;
    std::string error_type = "NoError";
    std::string error_message = "Congratulations!";
    double seconds = 0.0;
    try{
        const auto start_test{std::chrono::steady_clock::now()};
        task(args...);
        const auto end_test{std::chrono::steady_clock::now()};
        ended = true;
        std::chrono::duration<double> test_seconds = end_test - start_test;
        seconds = test_seconds.count();
    }
    catch(const Exception& my_error){
        error_type = my_error.what();
        error_message = my_error.message();
    }
    catch(std::exception* ex){
        error_type = ex->what();
        error_message = "Unknown";
    }
    catch(...){
        error_type = "Unknown";
        error_message = "Unknown";
    }
    return BaseTaskOutput(ended, error_type, error_message, seconds);
}
