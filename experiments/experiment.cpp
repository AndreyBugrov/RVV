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
    const num_type min_value = -100.0;
    const num_type max_value = 100.0;
    const std::map<std::string, FunctionIndex> function_name_to_index={
        std::pair<std::string, FunctionIndex>("vec_simple", FunctionIndex::kScalarProductSimple),
        std::pair<std::string, FunctionIndex>("vec_std", FunctionIndex::kScalarProductStd),
        std::pair<std::string, FunctionIndex>("matr_simple", FunctionIndex::kMatrixProductSimple),
        std::pair<std::string, FunctionIndex>("gram_schmidt_simple", FunctionIndex::kGramSchmidtSimple),
        std::pair<std::string, FunctionIndex>("qr_simple", FunctionIndex::kQRSimple),
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
    if(function_index==FunctionIndex::kScalarProductSimple||function_index==FunctionIndex::kScalarProductStd){
        correct_size = ArgumentNumber::kScalarProduct;
        if(arguments_size!=correct_size){
            throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected ", correct_size, " arguments for scalar product but passed ", arguments_size));
        }
        size_t vector_length = function_arguments[0];
        vector<num_type> a(vector_length);
        vector<num_type> b(vector_length);
        generate_rand_array(a.data(), vector_length, min_value, max_value);
        generate_rand_array(b.data(), vector_length, min_value, max_value);
        switch (function_index)
        {
        case FunctionIndex::kScalarProductSimple:
            output = run_experiment_task(experiment_count, scalar_product_simple, reset_scalar_product, a, b, vector_length);
            break;
        case FunctionIndex::kScalarProductStd:
            output = run_experiment_task(experiment_count, scalar_product_std, reset_scalar_product, a, b, vector_length);
            break;
        default:
            break;
        }
    }
    if(function_index==FunctionIndex::kMatrixProductSimple){
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
        generate_rand_array(a.data(), a_row_num*a_column_num, min_value, max_value);
        generate_rand_array(b.data(), a_column_num*b_column_num, min_value, max_value);
        generate_rand_array(c.data(), a_row_num*b_column_num, min_value, max_value);
        switch (function_index)
        {
        case FunctionIndex::kMatrixProductSimple:
            output = run_experiment_task(experiment_count, matrix_prod_base_simple, reset_matrix_product, a, b, c, a_row_num, a_column_num, b_column_num);
            break;
        default:
            break;
        }
    }
    if(function_index==FunctionIndex::kGramSchmidtSimple){
        correct_size = ArgumentNumber::kGramSchmidtProcess;
        if(arguments_size!=correct_size){
            throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected ", correct_size, " arguments for Gram-Schmidt process but passed ", arguments_size));
        }
        size_t vector_system_size = function_arguments[0];
        size_t vector_length = function_arguments[1];
        vector<vector<num_type>> vec_system(vector_system_size);
        for(size_t vec_ind = 0;vec_ind <vector_system_size;++vec_ind){
            vec_system[vec_ind].resize(vector_length);
            generate_rand_array(vec_system[vec_ind].data(), vector_length, min_value, max_value);
        }
        switch (function_index)
        {
        case FunctionIndex::kGramSchmidtSimple:
            output = run_experiment_task(experiment_count, gram_schmidt_base_simple, reset_gram_schmidt, vec_system);
            break;
        default:
            break;
        }
    }
    if(function_index==FunctionIndex::kQRSimple){
        correct_size = ArgumentNumber::kQRDecomposition;
        if(arguments_size!=correct_size){
            throw Exception(ErrorType::kWrongArgumentNumber, generate_string("Expected ", correct_size, " arguments for QR decomposition but passed ", arguments_size));
        }
        size_t row_num = function_arguments[0];
        size_t column_num = function_arguments[1];
        vector<num_type> matrix(row_num*column_num);
        vector<num_type> Q(row_num*column_num);
        vector<num_type> R(column_num*column_num);
        generate_rand_array(matrix.data(), row_num*column_num, min_value, max_value);
        switch (function_index)
        {
        case FunctionIndex::kQRSimple:
            output = run_experiment_task(experiment_count, QR_decomposition_base_simple, reset_qr, matrix, Q, R, row_num, column_num);
            break;
        default:
            break;
        }
    }
    return output;
}