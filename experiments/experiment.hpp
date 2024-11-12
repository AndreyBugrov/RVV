#pragma once

#include <iostream> // cerr

#include <string>
#include <vector>
#include <map>

#include "../tasks/base_task.hpp"  // BaseTaskOutput

#include "../common/exception.hpp"
#include "../common/single_logger.hpp" // logging
#include "../common/generators.hpp"  // only to generate error message

#include "../algorithms/scalar_product.hpp"
#include "../algorithms/matrix_product.hpp"
#include "../algorithms/gram_schmidt.hpp"
#include "../algorithms/qr_decomposition.hpp"

enum class FunctionIndex{
    kScalarProductSimple,
    kScalarProductStd,
    kMatrixProductSimple,
    kGramSchmidtSimple,
    kQRSimple,
};

// experiment_count is of int type because there are only std::stoi, not std::stoui
BaseTaskOutput run_experiment(int experiment_count, std::string function_name, std::vector<size_t> function_arguments);

void print_experiment_result(const BaseTaskOutput& output, std::ostream& stream);

double count_seconds(std::vector<double>& seconds);


// reset does not return anything so ResetFoo type != Foo type in general
template<typename Foo, typename ResetFoo, typename... Args>
BaseTaskOutput run_experiment_task(int experiment_num, Foo task, ResetFoo reset, Args... args){
    bool ended = false;
    std::string error_type = "NoError";
    std::string error_message = "";
    std::vector<double> seconds(experiment_num);
    for(int i=0;i<experiment_num;++i){
        try{
            ended = false;
            const auto start_test{std::chrono::steady_clock::now()};
            task(args...);
            const auto end_test{std::chrono::steady_clock::now()};
            reset(args...);
            ended = true;
            std::chrono::duration<double> experiment_seconds = end_test - start_test;
            seconds.push_back(experiment_seconds.count());
        }
        catch(const Exception& my_error){
            error_type = my_error.what();
            error_message = my_error.message();
            return BaseTaskOutput(ended, error_type, error_message, 0.0);
        }
        catch(const std::exception& ex){
            error_type = "std::exception";
            error_message = ex.what();
            return BaseTaskOutput(ended, error_type, error_message, 0.0);
        }
        catch(...){
            error_type = "Unknown";
            error_message = "Unknown";
            return BaseTaskOutput(ended, error_type, error_message, 0.0);
        }
    }
    return BaseTaskOutput(ended, error_type, error_message, count_seconds(seconds));
}
