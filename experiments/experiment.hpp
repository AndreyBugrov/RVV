#pragma once

#include <iostream> // cerr

#include <string>
#include <vector>
#include <map>

#include "../tasks/base_task.hpp"  // BaseTaskOutput

#include "../common/exception.hpp"
#include "../common/single_logger.hpp" // logging
#include "../common/generators.hpp"  // only to generate error message

#include "../algorithms/dot_product.hpp"
#include "../algorithms/matrix_product.hpp"
#include "../algorithms/gram_schmidt.hpp"
#include "../algorithms/qr_decomposition.hpp"

#include "reset.hpp"
#include "eigen.hpp"

enum class FunctionIndex{
    kDotProductSimple,
    kDotProductStd,
    kDotProductSimd,
    kDotProductUnrolling,
    kDotProductEnd, // end
    kMatrixProductSimple,
    kMatrixProductRow,
    kMatrixProductEnd, // end
    kGramSchmidtSimple,
    kGramSchmidtRow,
    kGramSchmidtSimd,
    kGramSchmidtUnrolling,
    kGramSchmidtEnd, // end
    kQRSimple,
    kQRRow,
    kQRRowRow,
    kQRSimd,
    kQRUnrolling,
    kQRDoubleUnrolling,
    kQRBlock,
    kQRBlockScalar,
    kQRInline,
    kQRMatrix,
    kQRHouseholderSimple,
    kQRHouseholderUnrolling,
    kQRInlinePar,
    kQREnd, // end
};

class ExperimentOutput : public BaseTaskOutput{
protected:
    double ref_seconds_;
public:
    ExperimentOutput(bool ended, std::string error_type, std::string error_msg, double seconds, double ref_seconds): BaseTaskOutput(ended, error_type, error_msg, seconds), ref_seconds_(ref_seconds){}
    ExperimentOutput(const ExperimentOutput& other): BaseTaskOutput(other), ref_seconds_(other.ref_seconds_){}
    double ref_time() const noexcept{return ref_seconds_;}
};

// experiment_count is of int type because there are only std::stoi, not std::stoui
ExperimentOutput run_experiment(int experiment_count, std::string function_name, std::vector<size_t> function_arguments, bool is_perf);

void print_experiment_result(const ExperimentOutput& output, std::ostream& stream);

double count_seconds(vector_num& seconds);  // need to be declared higher than run_experiment_task because it is used in it

// reset does not return anything so ResetFoo type != Foo type in general
template<typename Foo, typename RefFoo, typename ResetFoo, typename... Args>
ExperimentOutput run_experiment_task(int experiment_count, Foo task, RefFoo ref_task, ResetFoo reset, Args... args){
    bool no_errors = true;
    std::string error_type = "NoError";
    std::string error_message = "Everything is fine";
    vector_num task_seconds(experiment_count);
    vector_num ref_task_seconds(experiment_count);
    for(int i = 0; i < experiment_count && no_errors == true; ++i){
        try{
            no_errors = false;

            const auto ref_task_start{std::chrono::steady_clock::now()};
            ref_task(args...);
            const auto ref_task_end{std::chrono::steady_clock::now()};

            const auto task_start{std::chrono::steady_clock::now()};
            task(args...);
            const auto task_end{std::chrono::steady_clock::now()};

            std::chrono::duration<double> current_ref_task_seconds = ref_task_end - ref_task_start;
            ref_task_seconds.at(i) = current_ref_task_seconds.count();
            std::chrono::duration<double> current_task_seconds = task_end - task_start;
            task_seconds.at(i) = current_task_seconds.count();

            reset(args...);

            no_errors = true;
        }
        catch(const Exception& my_error){
            error_type = my_error.what();
            error_message = my_error.message();
        }
        catch(const std::exception& ex){
            error_type = "std::exception";
            error_message = ex.what();
        }
        catch(...){
            error_type = "Unknown";
            error_message = "Something went wrong";
        }
    }
    if(!no_errors){
        return ExperimentOutput(no_errors, error_type, error_message, 0.0, 0.0);
    }
    return ExperimentOutput(no_errors, error_type, error_message, count_seconds(task_seconds), count_seconds(ref_task_seconds));
}

void dumb_ref_task(const vector_num& matrix, vector_num& Q_matrix, vector_num& R_matrix, size_t row_count, size_t column_count);