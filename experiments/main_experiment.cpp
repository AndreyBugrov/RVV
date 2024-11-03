#include <iostream>
#include <string>

#include "experiment_task.hpp"
#include "../common/generators.hpp"
#include "../algorithms/qr_decomposition.hpp"

using std::vector;

int main(){
    // recognize function type
    // get arguments
    //map<string name, enum class function_index>
    //switch by function_index
    //run task

    // BaseTaskOutput output = run_experiment(abobus, 7.5, -3.2, false, 5);
    // std::cout<<"ended: ";
    // if(output.ended()){
    //     std::cout<<"true";
    // }else{
    //     std::cout<<"false";
    // }
    // std::cout<<"\n";
    // std::cout<<"seconds: "<<output.time()<<"\n";
    // std::cout<<"what: "<<output.what()<<"\n";
    // std::cout<<"error message: "<<output.error_message()<<"\n";

    size_t row_num = 121;
    size_t column_num = 215;
    vector<num_type> A(row_num*column_num);
    vector<num_type> Q_transposed(row_num*column_num);
    vector<num_type> R(row_num*column_num);
    generate_rand_array(A.data(), row_num*column_num, -100.0, 100.0);
    QR_decomposition_base_simple(A, Q_transposed, R, row_num, column_num);
    
    return 0;
}
