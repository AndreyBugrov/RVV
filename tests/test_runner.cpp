#include "test_runner.hpp"

const std::string delimiter = "--------------------------------------------------------------------------------\n";

const double kMinValue = -100.0;
const double kMaxValue = 100.0;
const size_t kMinLength = 256;
const size_t kMaxLength = 512;

static TestFunctionInput get_test_function_input() {
    return TestFunctionInput{kMinValue, kMaxValue, kMinLength, kMaxLength};
}

static void print_header(std::ostream& stream) {
    auto current_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    stream<<"RUN DATE: "<<std::ctime(&current_date)<<delimiter;
    stream<<"RUN PARAMETERS:\nVALUES: from "<< kMinValue<<" to "<<kMaxValue<<"\nLENGTH: from "<<kMinLength<<" to "<<kMaxLength<<"\n";
    stream << delimiter;
}

static void parse_passed_test_output(double test_time, std::ostream& stream){
    stream<<"PASSED\n";
    stream<<"TIME: "<<test_time<<" seconds\n";
}

static void parse_failed_test_output(const TestOutput& test_output, const std::string& test_task_name, std::vector<std::string>& wrong_test_names, std::ostream& stream) {
    wrong_test_names.push_back(test_task_name);
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

size_t run_tests(const std::vector<TestTask>& test_tasks, size_t task_count, std::ostream& stream) {
    print_header(stream);
    size_t passed = 0;
    size_t failed = 0;
    std::vector<std::string> wrong_test_names;
    const auto start_task{std::chrono::steady_clock::now()};
    for(TestTask task : test_tasks){
        stream<<"TEST "<<task.name()<<": ";
        TestFunctionInput input = get_test_function_input();
        TestOutput test_output = task.run(input);
        if(test_output.passed()){
            ++passed;
            parse_passed_test_output(test_output.time(), stream);
        }
        else{
            ++failed;
            parse_failed_test_output(test_output, task.name(), wrong_test_names, stream);
        }
        stream << delimiter;
    }
    const auto end_task{std::chrono::steady_clock::now()};
    std::chrono::duration<double> all_task_seconds = end_task - start_task;
    double total_seconds = all_task_seconds.count();
    stream<<"TOTAL:\n"<<"RUN:    "<<task_count<<"\nPASSED: "<<passed<<"\nFAILED: "<<failed<<"\nTIME:   "<<total_seconds<<"\n";
    if(failed){
        stream << delimiter;
        stream<<"FAILED TESTS: \n";
        for(std::string test_name : wrong_test_names){
            stream<<"\""<< test_name<<"\" ";
        }
        stream<<"\n";
    }
    return failed;
}
