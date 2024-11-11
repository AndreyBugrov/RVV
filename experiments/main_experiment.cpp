#include <string>
#include <vector>
#include <iostream>

#include "experiment.hpp"  // run_experiment

using std::vector;

int main(int argc, char* argv[]){
    const int argc_min = 5;
    if(argc < argc_min){
        throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected at least ", argc_min, "arguments but passed only ", argc));
    }
    try{
        int experiment_count = std::stoi(argv[1]);
        std::string function_name = argv[2];
        std::vector<size_t> function_arguments;
        for(int i = 3, inc_argc = argc-1;i<inc_argc;++i){
            function_arguments.push_back(std::stol(argv[i]));
        }
        SingleLogger* logger = SingleLogger::get_instance(); ///////////////////////// GlobalLogger instead of SingleLogger
        BaseTaskOutput output = run_experiment(experiment_count, function_name, function_arguments);
        print_experiment_result(output, std::cout);
    }
    catch(Exception& ex){
        std::cerr<<ex.what()<<"\n";
        std::cerr<<ex.message()<<"\n";
    }
    catch(std::exception& ex){
        std::cerr<<ex.what()<<"\n";
    }
}
