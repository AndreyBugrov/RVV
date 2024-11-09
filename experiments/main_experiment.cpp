#include <string>
#include <vector>
#include <iostream>

#include "experiment.hpp"  // run_experiment

using std::vector;

int main(int argc, char* argv[]){
    const int argc_min = 3;
    if(argc < argc_min){
        throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected at least ", argc_min, "arguments but passed only ", argc));
    }
    std::string function_name = argv[1];
    std::vector<size_t> function_arguments;
    for(int i = 2;i<argc;++i){
        function_arguments.push_back(std::stol(argv[i]));
    }
    try{
        BaseTaskOutput output = run_experiment(function_name, function_arguments);
        print_experiment_result(output, std::cout);
    }catch(Exception& ex){
        std::cerr<<ex.what()<<"\n";
        std::cerr<<ex.message()<<"\n";
    }
}
