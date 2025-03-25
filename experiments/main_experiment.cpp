#include <string>
#include <vector>
#include <iostream>

#include "experiment.hpp"  // run_experiment

using std::vector;

int main(int argc, char* argv[]){
    // experiment count, function name, function arguments, .log file path
    const int kMinArgumentsCount = 5;
    if(argc < kMinArgumentsCount){
        std::string arguments;
        for(int argument_index = 1; argument_index<argc; ++argument_index){
            arguments += std::to_string(argument_index) + ". " + argv[argument_index] + "\n";
        }
        Exception ex(ErrorType::kIncorrectArgumentCount, generate_string("Expected at least ", kMinArgumentsCount, " arguments but passed ", argc, ". Arguments:\n", arguments));
        std::cerr<<ex.what()<<"\n";
        std::cerr<<ex.message()<<"\n";
        return argc;
    }
    try{
        int experiment_count = std::stoi(argv[1]);
        std::string function_name = argv[2];
        std::vector<size_t> function_arguments;
        size_t function_arguments_count = argc-1;
        for(size_t i = 3; i<function_arguments_count; ++i){
            function_arguments.push_back(std::stol(argv[i]));
        }
        // SingleLogger* logger = SingleLogger::get_instance(); ///////////////////////// GlobalLogger instead of SingleLogger
        BaseTaskOutput output = run_experiment(experiment_count, function_name, function_arguments);
        print_experiment_result(output, std::cout);
    }
    catch(Exception& ex){
        std::cerr<<ex.what()<<"\n";
        std::cerr<<ex.message()<<"\n";
    }
    catch(std::exception& ex){
        std::cout<<"std::exception"<<"\n";
        std::cerr<<ex.what()<<"\n";
    }
    catch(...){
        std::cout<<"Unknown Exception"<<"\n";
    }
}
