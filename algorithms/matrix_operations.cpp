#include "matrix_operations.hpp"

vector<num_type> transpose_matrix(const vector<num_type>& matr, size_t row_num, size_t column_num){
    vector<num_type> matr_T(column_num*row_num);
    for(size_t i=0;i<column_num;++i){
        for(size_t j=0;j<row_num;++j){
            matr_T[i*row_num+j]=matr[j*column_num+i];
        }
    }
    return matr_T;
}