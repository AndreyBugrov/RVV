#include <iostream>
#include <string>

#include "experiment_task.hpp"  // run_experiment
#include "../common/single_logger.hpp"  // logger
#include "../algorithms/qr_decomposition.hpp"  // qr decomposition

#include "../algorithms/matrix_operations.hpp"  // transpose for check
#include "../tests/assert.hpp"  // assert namespace for check

using std::vector;

int main(){
    // recognize function type
    // get arguments
    //map<string name, enum class function_index>
    //switch by function_index
    //run task



    size_t row_num = 121;
    size_t column_num = 215;
    vector<num_type> A(row_num*column_num);
    vector<num_type> Q_transposed(column_num*row_num);
    vector<num_type> R(column_num*column_num);
    generate_rand_array(A.data(), row_num*column_num, -100.0, 100.0);
    SingleLogger* logger = SingleLogger::get_instance();
    logger->set_log_level(LoggerLevel::kDebug);
    BaseTaskOutput output = run_experiment(QR_decomposition_base_simple, A, Q_transposed, R, row_num, column_num);
    std::cout<<"ended: ";
    if(output.ended()){
        std::cout<<"true\n";
    }else{
        std::cout<<"false\n";
    }
    std::cout<<"seconds: "<<output.time()<<"\n";
    std::cout<<"what: "<<output.what()<<"\n";
    std::cout<<"error message: "<<output.error_message()<<"\n";
    vector<num_type> Q = transpose_matrix(Q_transposed, column_num, row_num);
    vector<num_type> result_A(row_num*column_num);
    matrix_prod_base_simple(Q, R, result_A, row_num, column_num, column_num);
    AssertionResult result = assert::assert_iterable_containers_eq(A, result_A, row_num*column_num);
    std::cout<<"result is: ";
    if(result){
        std::cout<<"true\n";
    }else{
        std::cout<<"false\n";
    }
    std::cout<<result.error_message()<<"\n";
    return 0;
}
