#include "test_runner.hpp"

const std::string TestRunner::delimiter = "--------------------------------------------------------------------------------\n";

const double TestRunner::kMinValue_ = -100.0;
const double TestRunner::kMaxValue_ = 100.0;
const size_t TestRunner::kMinLength_ = 256;
const size_t TestRunner::kMaxLength_ = 512;

TestFunctionInput TestRunner::get_test_function_input() const{
    return TestFunctionInput{kMinValue_, kMaxValue_, kMinLength_, kMaxLength_};
}

TestFunctionInput TestRunner::create_and_store_input(){
    TestFunctionInput input = get_test_function_input();
    task_inputs_.push_back(input);
    return input;
}

void TestRunner::init_run(std::ostream& stream){
    print_header(stream);
    task_inputs_.clear();
}

void TestRunner::print_header(std::ostream& stream) const {
    auto current_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    //const std::string delimiter = "--------------------------------------------------------------------------------\n";
    stream<<"RUN DATE: "<<std::ctime(&current_date)<<delimiter;
    stream<<"RUN PARAMETERS:\nVALUES: from "<< kMinValue_<<" to "<<kMaxValue_<<"\nLENGTH: from "<<kMinLength_<<" to "<<kMaxLength_<<"\n";
    stream << delimiter;
}

void TestRunner::run_all(std::ostream& stream) {
    init_run(stream);
    size_t passed = 0;
    size_t failed = 0;
    std::vector<std::string> wrong_test_names;
    const auto start_task{std::chrono::steady_clock::now()};
    for(TestTask task : (*test_tasks_)){
        stream<<"TEST "<<task.name()<<": ";
        TestFunctionInput input = create_and_store_input();
        TestOutput test_output = task.run(input);
        if(test_output.passed()){
            stream<<"PASSED\n";
            ++passed;
            stream<<"TIME: "<<test_output.time()<<" seconds\n";
        }
        else{
            wrong_test_names.push_back(task.name());
            ++failed;
            stream<<"FAILED DUE TO ";
            if(test_output.ended()){
                stream<<"ERROR\n";
                stream<<"TIME: "<<test_output.time()<<" seconds\n";
                stream<<"ERROR: "<<test_output.error_message()<<"\n";
            }else{
                stream<<"EXCEPTION\n";
                stream<<"EXCEPTION TYPE: "<<test_output.what()<<"\n";
                stream<<"EXCEPTION MESSAGE: "<<test_output.error_message()<<"\n";
            }
        }
        stream << delimiter;
    }
    const auto end_task{std::chrono::steady_clock::now()};
    std::chrono::duration<double> all_task_seconds = end_task - start_task;
    double total_seconds = all_task_seconds.count();
    stream<<"TOTAL:\n"<<"RUN:    "<<task_num_<<"\nPASSED: "<<passed<<"\nFAILED: "<<failed<<"\nTIME:   "<<total_seconds<<"\n";
    if(wrong_test_names.size()){
        stream << delimiter;
        stream<<"FAILED TESTS: \n";
        for(std::string test_name : wrong_test_names){
            stream<<"\""<< test_name<<"\" ";
        }
        stream<<"\n";
    }
}

void TestRunner::run_by_name(const std::string& name, std::ostream& stream) {
    int index = -1;
    for(size_t i = 0; i<task_num_; i++){
        if(test_tasks_->at(i).name() == name){
            index = i;
            break;
        }
    }
    if(index == -1){
        throw Exception(ErrorType::kValueError, generate_string("Unexpected test task name: ", name));
    }
    init_run(stream);
    stream<<"TEST "<<test_tasks_->at(index).name()<<": ";
    TestFunctionInput input = create_and_store_input();
    TestOutput test_output = test_tasks_->at(index).run(input);
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
