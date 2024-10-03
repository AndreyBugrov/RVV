#include "test_runner.hpp"

void TestRunner::run_all(std::ostream& stream){
    double min_value = -50.0, max_value = 50.0;
    size_t min_length = 128, max_length = 256;
    TestFunctionInput input{min_value, max_value, min_length, max_length};
    auto current_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const std::string delimiter = "--------------------------------------------------------------------------------\n";
    stream<<"TEST DATE: "<<std::ctime(&current_date)<<delimiter;
    stream<<"TEST PARAMETERS\nVALUES: from "<< min_value<<" to "<<max_value<<"\nLENGTH: from "<<min_length<<" to "<<max_length<<"\n";
    size_t passed = 0;
    size_t failed = 0;
    const auto start_task{std::chrono::steady_clock::now()};
    for(TestTask task : (*test_tasks_)){
        stream << delimiter;
        stream<<"TEST "<<task.name()<<": ";
        TestOutput test_output = task.run(input);
        if(test_output.passed()){
            stream<<"PASSED\n";
            ++passed;
            stream<<"TIME: "<<test_output.time()<<" seconds\n";
        }
        else{
            stream<<"FAILED\n";
            ++failed;
            if(test_output.what()!=""){
                stream<<"ERROR: "<<test_output.what()<<"\n";
            }else{
                stream<<"TIME: "<<test_output.time()<<" seconds\n";
            }
        }
    }
    const auto end_task{std::chrono::steady_clock::now()};
    std::chrono::duration<double> all_task_seconds = end_task - start_task;
    double total_seconds = all_task_seconds.count();
    stream<<delimiter<<"TEST TOTAL\n"<<"RUN:    "<<passed+failed<<"\nPASSED: "<<passed<<"\nFAILED: "<<failed<<"\nTIME:   "<<total_seconds<<"\n";
}