#include "test_runner.hpp"

void TestRunner::run_all(std::ostream& stream){
    TestFunctionInput input{-50.0, 50.0, 128, 256};
    auto current_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const std::string delimiter = "--------------------------------------------------------------------------------\n";
    stream<<"TEST DATE: "<<std::ctime(&current_date)<<"\n"<<delimiter;
    stream<<"TEST PARAMETERS:\nVALUES: from -50.0 to 50.0\n\tLENGTH: from 128 to 256\n";
    size_t passed = 0;
    size_t failed = 0;
    const auto start_task{std::chrono::steady_clock::now()};
    for(TestTask task : (*test_tasks_)){
        stream << delimiter;
        stream<<task.name()<<"\n";
        TestOutput test_output = task.run(input);
        if(test_output.passed()){
            stream<<"PASSED\n";
            ++passed;
        }
        else{
            stream<<"FAILED\n";
            ++failed;
            if(test_output.what()!=""){
                stream<<"ERRORS:\n"<<test_output.what()<<"\n";
            }else{
                stream<<"TIME: "<<test_output.time()<<"\n";
            }
        }
    }
    const auto end_task{std::chrono::steady_clock::now()};
    double total_seconds = (end_task-start_task).count();
    stream<<delimiter<<"TEST TOTAL:\n"<<"RUN: "<<passed+failed<<"\nPASSED: "<<passed<<"\nFAILED:"<<failed<<"TIME: "<<total_seconds<<"\n";
}