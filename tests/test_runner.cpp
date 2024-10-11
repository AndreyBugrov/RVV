#include "test_runner.hpp"

const std::string TestRunner::delimiter = "--------------------------------------------------------------------------------\n";

TestFunctionInput TestRunner::get_test_function_input(double min_value = -50.0, double max_value = 50.0, size_t min_length = 128, size_t max_length = 256) const{
    return TestFunctionInput{min_value, max_value, min_length, max_length};
}

void TestRunner::run_all(std::ostream& stream){
    double min_value = -50.0, max_value = 50.0;
    size_t min_length = 128, max_length = 256;
    auto current_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    //const std::string delimiter = "--------------------------------------------------------------------------------\n";
    stream<<"TEST DATE: "<<std::ctime(&current_date)<<delimiter;
    stream<<"TEST PARAMETERS\nVALUES: from "<< min_value<<" to "<<max_value<<"\nLENGTH: from "<<min_length<<" to "<<max_length<<"\n";
    size_t passed = 0;
    size_t failed = 0;
    const auto start_task{std::chrono::steady_clock::now()};
    for(TestTask task : (*test_tasks_)){
        stream << delimiter;
        stream<<"TEST "<<task.name()<<": ";
        TestOutput test_output = task.run(get_test_function_input(min_value, max_value, min_length, max_length));
        if(test_output.passed()){
            stream<<"PASSED\n";
            ++passed;
            stream<<"TIME: "<<test_output.time()<<" seconds\n";
        }
        else{
            stream<<"FAILED\n";
            ++failed;
            if(test_output.ended()){
                stream<<"TIME: "<<test_output.time()<<" seconds\n";
                stream<<"ERROR: "<<test_output.error_message()<<"\n";
            }else{
                stream<<"EXCEPTION TYPE: "<<test_output.what()<<"\n";
                stream<<"EXCEPTION MESSAGE: "<<test_output.error_message()<<"\n";
            }
        }
    }
    const auto end_task{std::chrono::steady_clock::now()};
    std::chrono::duration<double> all_task_seconds = end_task - start_task;
    double total_seconds = all_task_seconds.count();
    stream<<delimiter<<"TEST TOTAL\n"<<"RUN:    "<<task_num_<<"\nPASSED: "<<passed<<"\nFAILED: "<<failed<<"\nTIME:   "<<total_seconds<<"\n";
}

void TestRunner::run_by_name(const std::string& name, std::ostream& stream = std::cout){
    double min_value = -50.0, max_value = 50.0;
    size_t min_length = 128, max_length = 256;
    TestFunctionInput input = get_test_function_input(min_value, max_value, min_length, max_length);
    stream << delimiter;
    int index = -1;
    for(int i = 0; i<task_num_; i++){
        if(test_tasks_->operator[](i).name() == name){
            index = i;
            break;
        }
    }
    if(index == -1){
        throw Exception(ErrorType::kValueError, generate_string("Unexpected test task name: ", name));
    }
    TestTask task = test_tasks_->operator[](index);
    stream<<"TEST "<<task.name()<<": ";
    TestOutput test_output = task.run(get_test_function_input(min_value, max_value, min_length, max_length));
    if(test_output.passed()){
        stream<<"PASSED\n";
        stream<<"TIME: "<<test_output.time()<<" seconds\n";
    }
    else{
        stream<<"FAILED\n";
        if(test_output.ended()){
            stream<<"TIME: "<<test_output.time()<<" seconds\n";
            stream<<"ERROR: "<<test_output.error_message()<<"\n";
        }else{
            stream<<"EXCEPTION TYPE: "<<test_output.what()<<"\n";
            stream<<"EXCEPTION MESSAGE: "<<test_output.error_message()<<"\n";
        }
    }
}
