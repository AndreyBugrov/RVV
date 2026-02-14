#include "experiment.hpp"

using std::vector;

void print_experiment_result(const ExperimentOutput& output, std::ostream& stream){
    if(!output.ended()){
        std::cerr<<output.what()<<"\n";
        std::cerr<<output.error_message()<<"\n";
    }
    stream<<output.time()<<"\n";
    stream<<output.ref_time()<<"\n";
}

double count_seconds(vector_num& seconds){
    if(seconds.size()==0){
        throw Exception(ErrorType::kValueError, generate_string("Seconds list is empty"));
    }
    if(seconds.size()==1){
        return seconds[0];
    }
    double result_seconds = 0.0;
    double max_seconds = 0.0;
    size_t max_seconds_index = 0;
    for(size_t i=0;i<seconds.size();++i){
        if(max_seconds< seconds[i]){
            max_seconds = seconds[i];
            max_seconds_index = i;
        }
    }
    for(size_t i=0;i<seconds.size();++i){
        if(i!=max_seconds_index){
           result_seconds += seconds[i];
        }
    }
    result_seconds /= (seconds.size()-1);
    return result_seconds;
}

ExperimentOutput run_experiment(int experiment_count, std::string function_name, std::vector<size_t> function_arguments, bool is_perf){
    const num_type kMinValue = -100.0;
    const num_type kMaxValue = 100.0;
    const std::map<std::string, FunctionIndex> function_name_to_index={
        std::pair<std::string, FunctionIndex>("vec_p_sim", FunctionIndex::kDotProductSimple),
        std::pair<std::string, FunctionIndex>("vec_p_std", FunctionIndex::kDotProductStd),
        std::pair<std::string, FunctionIndex>("vec_p_simd", FunctionIndex::kDotProductSimd),
        std::pair<std::string, FunctionIndex>("vec_p_urol", FunctionIndex::kDotProductUnrolling),
        std::pair<std::string, FunctionIndex>("mat_p_sim", FunctionIndex::kMatrixProductSimple),
        std::pair<std::string, FunctionIndex>("mat_p_row_sim", FunctionIndex::kMatrixProductRow),
        std::pair<std::string, FunctionIndex>("gs_p_sim", FunctionIndex::kGramSchmidtSimple),
        std::pair<std::string, FunctionIndex>("gs_p_row_sim", FunctionIndex::kGramSchmidtRow),
        std::pair<std::string, FunctionIndex>("gs_p_simd", FunctionIndex::kGramSchmidtSimd),
        std::pair<std::string, FunctionIndex>("gs_p_urol", FunctionIndex::kGramSchmidtUnrolling),
        std::pair<std::string, FunctionIndex>("qr_d_sim", FunctionIndex::kQRSimple),
        std::pair<std::string, FunctionIndex>("qr_d_row_sim", FunctionIndex::kQRRow),
        std::pair<std::string, FunctionIndex>("qr_d_row_row", FunctionIndex::kQRRowRow),
        std::pair<std::string, FunctionIndex>("qr_d_simd", FunctionIndex::kQRSimd),
        std::pair<std::string, FunctionIndex>("qr_d_urol", FunctionIndex::kQRUnrolling),
        std::pair<std::string, FunctionIndex>("qr_d_drol", FunctionIndex::kQRDoubleUnrolling),
        std::pair<std::string, FunctionIndex>("qr_d_block", FunctionIndex::kQRBlock),
        std::pair<std::string, FunctionIndex>("qr_d_dot", FunctionIndex::kQRBlockScalar),
        std::pair<std::string, FunctionIndex>("qr_d_inl", FunctionIndex::kQRInline),
        std::pair<std::string, FunctionIndex>("qr_d_matr", FunctionIndex::kQRMatrix),
        std::pair<std::string, FunctionIndex>("qr_d_hh_sim", FunctionIndex::kQRHouseholderSimple),
        std::pair<std::string, FunctionIndex>("qr_d_hh_urol", FunctionIndex::kQRHouseholderUnrolling),
    };
    enum ArgumentNumber{
        kDotProduct = 1,
        kMatrixProduct = 3,
        kGramSchmidtProcess = 2,
        kQRDecomposition = 2,
    };
    FunctionIndex function_index = function_name_to_index.at(function_name);
    size_t arguments_size = function_arguments.size();
    size_t correct_size;

    // if(static_cast<int>(function_index)>=static_cast<int>(FunctionIndex::kDotProductSimple) && static_cast<int>(function_index) < static_cast<int>(FunctionIndex::kDotProductEnd)){
    //     correct_size = ArgumentNumber::kDotProduct;
    //     if(arguments_size!=correct_size){
    //         throw Exception(ErrorType::kIncorrectArgumentCount, generate_string("Expected ", correct_size, " arguments for dot product but passed ", arguments_size));
    //     }
    //     size_t vector_length = function_arguments[0];
    //     vector<num_type> a(vector_length);
    //     vector<num_type> b(vector_length);
    //     generate_rand_array(a.data(), vector_length, kMinValue, kMaxValue);
    //     generate_rand_array(b.data(), vector_length, kMinValue, kMaxValue);
    //     std::function<num_type (const vector_num&, const vector_num&, size_t)> foo;
    //     switch (function_index)
    //     {
    //     case FunctionIndex::kDotProductSimple:
    //         foo = dot_product_simple_unsafe;
    //         break;
    //     case FunctionIndex::kDotProductStd:
    //         foo = dot_product_std;
    //         break;
    //     case FunctionIndex::kDotProductSimd:
    //         foo = dot_product_simd;
    //         break;
    //     case FunctionIndex::kDotProductUnrolling:
    //         foo = dot_product_unrolling;
    //         break;
    //     default:
    //         break;
    //     }
    //     return run_experiment_task(experiment_count, foo, reset_dot_product, a, b, vector_length);
    // }
    // if(static_cast<int>(function_index) > static_cast<int>(FunctionIndex::kDotProductEnd) && static_cast<int>(function_index) < static_cast<int>(FunctionIndex::kMatrixProductEnd)){
    //     correct_size = ArgumentNumber::kMatrixProduct;
    //     if(arguments_size!=correct_size){
    //         throw Exception(ErrorType::kIncorrectArgumentCount, generate_string("Expected ", correct_size, " arguments for matrix product but passed ", arguments_size));
    //     }
    //     size_t a_row_num = function_arguments[0];
    //     size_t a_column_num = function_arguments[1];
    //     size_t b_column_num = function_arguments[2];
    //     vector<num_type> a(a_row_num*a_column_num);
    //     vector<num_type> b(a_column_num*b_column_num);
    //     vector<num_type> c(a_row_num*b_column_num);
    //     generate_rand_array(a.data(), a_row_num*a_column_num, kMinValue, kMaxValue);
    //     generate_rand_array(b.data(), a_column_num*b_column_num, kMinValue, kMaxValue);
    //     generate_rand_array(c.data(), a_row_num*b_column_num, kMinValue, kMaxValue);
    //     std::function<void(const vector_num&, const vector_num&, vector_num&, size_t, size_t, size_t)> foo;
    //     switch (function_index)
    //     {
    //     case FunctionIndex::kMatrixProductSimple:
    //         foo = matrix_product_base_simple;
    //         break;
    //     case FunctionIndex::kMatrixProductRow:
    //         foo = matrix_product_row_simple;
    //         break;
    //     default:
    //         break;
    //     }
    //     return run_experiment_task(experiment_count, foo, reset_matrix_product, a, b, c, a_row_num, a_column_num, b_column_num);
    // }
    // if(static_cast<int>(function_index) > static_cast<int>(FunctionIndex::kMatrixProductEnd) && static_cast<int>(function_index) < static_cast<int>(FunctionIndex::kGramSchmidtEnd)){
    //     correct_size = ArgumentNumber::kGramSchmidtProcess;
    //     if(arguments_size!=correct_size){
    //         throw Exception(ErrorType::kIncorrectArgumentCount, generate_string("Expected ", correct_size, " arguments for Gram-Schmidt process but passed ", arguments_size));
    //     }
    //     size_t vector_system_size = function_arguments[0];
    //     size_t vector_length = function_arguments[1];
    //     switch (function_index)
    //     {
    //     case FunctionIndex::kGramSchmidtSimple:
    //         {
    //             vector<vector<num_type>> vec_system(vector_system_size);
    //             for(size_t vec_ind = 0;vec_ind <vector_system_size;++vec_ind){
    //                 vec_system[vec_ind].resize(vector_length);
    //                 generate_rand_array(vec_system[vec_ind].data(), vector_length, kMinValue, kMaxValue);
    //             }
    //             return run_experiment_task(experiment_count, gram_schmidt_base_simple, reset_gram_schmidt, vec_system);
    //         }
    //         break;
    //     case FunctionIndex::kGramSchmidtRow:
    //         {
    //             vector_num vec_system(vector_system_size*vector_length);
    //             generate_rand_array(vec_system.data(), vector_system_size*vector_length, kMinValue, kMaxValue);
    //             return run_experiment_task(experiment_count, gram_schmidt_matrix_simple, reset_inplace_gram_schmidt, vec_system, vector_system_size, vector_length);
    //         }
    //         break;
    //     case FunctionIndex::kGramSchmidtSimd:
    //         {
    //             vector_num vec_system(vector_system_size*vector_length);
    //             generate_rand_array(vec_system.data(), vector_system_size*vector_length, kMinValue, kMaxValue);
    //             return run_experiment_task(experiment_count, gram_schmidt_matrix_simd, reset_inplace_gram_schmidt, vec_system, vector_system_size, vector_length);
    //         }
    //         break;
    //     case FunctionIndex::kGramSchmidtUnrolling:
    //         {
    //             vector_num vec_system(vector_system_size*vector_length);
    //             generate_rand_array(vec_system.data(), vector_system_size*vector_length, kMinValue, kMaxValue);
    //             return run_experiment_task(experiment_count, gram_schmidt_matrix_unrolling, reset_inplace_gram_schmidt, vec_system, vector_system_size, vector_length);
    //         }
    //     default:
    //         break;
    //     }
    // }
    if(static_cast<int>(function_index) > static_cast<int>(FunctionIndex::kGramSchmidtEnd) && static_cast<int>(function_index) < static_cast<int>(FunctionIndex::kQREnd)){
        correct_size = ArgumentNumber::kQRDecomposition;
        if(arguments_size!=correct_size){
            throw Exception(ErrorType::kIncorrectArgumentCount, generate_string("Expected ", correct_size, " arguments for QR decomposition but passed ", arguments_size));
        }
        size_t row_num = function_arguments[0];
        size_t column_num = function_arguments[1];
        vector<num_type> matrix(row_num*column_num);
        vector<num_type> Q(row_num*column_num);
        vector<num_type> R(column_num*column_num);
        generate_rand_array(matrix.data(), row_num*column_num, kMinValue, kMaxValue);
        std::function<void(const vector_num&, vector_num&, vector_num&, size_t, size_t)> foo;
        switch (function_index)
        {
        case FunctionIndex::kQRSimple:
            foo = QR_decomposition_base_simple;
            break;
        case FunctionIndex::kQRRow:
            foo = QR_decomposition_row_product_simple;
            break;
        case FunctionIndex::kQRRowRow:
            foo = QR_decomposition_row_product_matrix_process_simple;
            break;
        case FunctionIndex::kQRSimd:
            foo = QR_decomposition_simd;
            break;
        case FunctionIndex::kQRUnrolling:
            foo = QR_decomposition_unrolling;
            break;
        case FunctionIndex::kQRDoubleUnrolling:
            foo = QR_decomposition_double_unrolling;
            break;
        case FunctionIndex::kQRBlock:
            foo = QR_decomposition_block;
            break;
        case FunctionIndex::kQRBlockScalar:
            foo = QR_decomposition_block_scalar;
            break;
        case FunctionIndex::kQRInline:
            foo = QR_decomposition_block_scalar_inline;
            break;
        case FunctionIndex::kQRMatrix:
            foo = QR_decomposition_full_matrix;
            break;
        case FunctionIndex::kQRHouseholderSimple:
            foo = QR_decomposition_base_householder;
            break;
        case FunctionIndex::kQRHouseholderUnrolling:
            foo = QR_decomposition_householder_unrolling;
            break;
        default:
            break;
        }
        if (is_perf){
            return run_experiment_task(experiment_count, foo, dumb_ref_task, reset_qr, matrix, Q, R, row_num, column_num);
        }
        int thread_count = omp_get_max_threads();
        omp_set_num_threads(thread_count);
        return run_experiment_task(experiment_count, foo, eigen_qr_decomposition, reset_qr, matrix, Q, R, row_num, column_num);
    }
    return ExperimentOutput(false, "UnknownError", "Was not ran", 0.0, 0.0);
}

void dumb_ref_task(const vector_num& matrix, vector_num& Q_matrix, vector_num& R_matrix, size_t row_count, size_t column_count){}