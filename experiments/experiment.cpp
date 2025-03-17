#include "experiment.hpp"

using std::vector;

void print_experiment_result(const BaseTaskOutput& output, std::ostream& stream){
    if(!output.ended()){
        std::cerr<<output.what()<<"\n";
        std::cerr<<output.error_message()<<"\n";
    }
    stream<<output.time()<<"\n";
}

double count_seconds(std::vector<double>& seconds){
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

BaseTaskOutput run_experiment(int experiment_count, std::string function_name, std::vector<size_t> function_arguments){
    const num_type kMinValue = -100.0;
    const num_type kMaxValue = 100.0;
    const std::map<std::string, FunctionIndex> function_name_to_index={
        std::pair<std::string, FunctionIndex>("vec_p_sim", FunctionIndex::kScalarProductSimple),
        std::pair<std::string, FunctionIndex>("vec_p_std", FunctionIndex::kScalarProductStd),
        std::pair<std::string, FunctionIndex>("mat_p_sim", FunctionIndex::kMatrixProductSimple),
        std::pair<std::string, FunctionIndex>("mat_p_row", FunctionIndex::kMatrixProductRow),
        std::pair<std::string, FunctionIndex>("gs_p_sim", FunctionIndex::kGramSchmidtSimple),
        std::pair<std::string, FunctionIndex>("gs_p_row", FunctionIndex::kGramSchmidtRow),
        std::pair<std::string, FunctionIndex>("qr_d_sim", FunctionIndex::kQRSimple),
        std::pair<std::string, FunctionIndex>("qr_d_row", FunctionIndex::kQRRow),
        std::pair<std::string, FunctionIndex>("qr_d_row_std", FunctionIndex::kQRRowRow),
    };
    enum ArgumentNumber{
        kScalarProduct = 1,
        kMatrixProduct = 3,
        kGramSchmidtProcess = 2,
        kQRDecomposition = 2,
    };
    FunctionIndex function_index = function_name_to_index.at(function_name);
    size_t arguments_size = function_arguments.size();
    size_t correct_size;

    BaseTaskOutput output(false, "UnknownError", "Was not ran", 0.0);
    if(function_index==FunctionIndex::kScalarProductSimple || function_index==FunctionIndex::kScalarProductStd){
        correct_size = ArgumentNumber::kScalarProduct;
        if(arguments_size!=correct_size){
            throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected ", correct_size, " arguments for scalar product but passed ", arguments_size));
        }
        size_t vector_length = function_arguments[0];
        vector<num_type> a(vector_length);
        vector<num_type> b(vector_length);
        generate_rand_array(a.data(), vector_length, kMinValue, kMaxValue);
        generate_rand_array(b.data(), vector_length, kMinValue, kMaxValue);
        std::function<num_type (const vector_num&, const vector_num&, size_t)> foo;
        switch (function_index)
        {
        case FunctionIndex::kScalarProductSimple:
            foo = dot_product_simple;
            break;
        case FunctionIndex::kScalarProductStd:
            foo = dot_product_std;
            break;
        default:
            break;
        }
        return run_experiment_task(experiment_count, foo, reset_dot_product, a, b, vector_length);
    }
    if(function_index==FunctionIndex::kMatrixProductSimple || function_index==FunctionIndex::kMatrixProductRow){
        correct_size = ArgumentNumber::kMatrixProduct;
        if(arguments_size!=correct_size){
            throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected ", correct_size, " arguments for scalar product but passed ", arguments_size));
        }
        size_t a_row_num = function_arguments[0];
        size_t a_column_num = function_arguments[1];
        size_t b_column_num = function_arguments[2];
        vector<num_type> a(a_row_num*a_column_num);
        vector<num_type> b(a_column_num*b_column_num);
        vector<num_type> c(a_row_num*b_column_num);
        generate_rand_array(a.data(), a_row_num*a_column_num, kMinValue, kMaxValue);
        generate_rand_array(b.data(), a_column_num*b_column_num, kMinValue, kMaxValue);
        generate_rand_array(c.data(), a_row_num*b_column_num, kMinValue, kMaxValue);
        std::function<void(const vector_num&, const vector_num&, vector_num&, size_t, size_t, size_t)> foo;
        switch (function_index)
        {
        case FunctionIndex::kMatrixProductSimple:
            foo = matrix_product_base_simple;
            break;
        case FunctionIndex::kMatrixProductRow:
            foo = matrix_product_row_simple;
            break;
        default:
            break;
        }
        return run_experiment_task(experiment_count, foo, reset_matrix_product, a, b, c, a_row_num, a_column_num, b_column_num);
    }
    if(function_index==FunctionIndex::kGramSchmidtSimple || function_index==FunctionIndex::kGramSchmidtRow){
        correct_size = ArgumentNumber::kGramSchmidtProcess;
        if(arguments_size!=correct_size){
            throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected ", correct_size, " arguments for Gram-Schmidt process but passed ", arguments_size));
        }
        size_t vector_system_size = function_arguments[0];
        size_t vector_length = function_arguments[1];
        vector<vector<num_type>> vec_system(vector_system_size);
        for(size_t vec_ind = 0;vec_ind <vector_system_size;++vec_ind){
            vec_system[vec_ind].resize(vector_length);
            generate_rand_array(vec_system[vec_ind].data(), vector_length, kMinValue, kMaxValue);
        }
        switch (function_index)
        {
        case FunctionIndex::kGramSchmidtSimple:
            {
                vector<vector<num_type>> vec_system(vector_system_size);
                for(size_t vec_ind = 0;vec_ind <vector_system_size;++vec_ind){
                    vec_system[vec_ind].resize(vector_length);
                    generate_rand_array(vec_system[vec_ind].data(), vector_length, kMinValue, kMaxValue);
                }
                return run_experiment_task(experiment_count, gram_schmidt_base_simple, reset_gram_schmidt, vec_system);
            }
            break;
        case FunctionIndex::kGramSchmidtRow:
            {
                vector_num vec_system(vector_system_size*vector_length);
                generate_rand_array(vec_system.data(), vector_system_size*vector_length, kMinValue, kMaxValue);
                return run_experiment_task(experiment_count, gram_schmidt_matrix_simple_inplace, reset_inplace_gram_schmidt, vec_system);
            }
            break;
        default:
            break;
        }
    }
    if(function_index==FunctionIndex::kQRSimple || function_index==FunctionIndex::kQRRow || function_index==FunctionIndex::kQRRowRow){
        correct_size = ArgumentNumber::kQRDecomposition;
        if(arguments_size!=correct_size){
            throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected ", correct_size, " arguments for QR decomposition but passed ", arguments_size));
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
        default:
            break;
        }
        return run_experiment_task(experiment_count, foo, reset_qr, matrix, Q, R, row_num, column_num);
    }
    return output;
}