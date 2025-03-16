#include "matrix_operations.hpp"

vector<num_type> transpose_matrix(const vector<num_type>& matr, size_t row_count, size_t column_count){
    vector<num_type> matr_T(column_count*row_count);
    for(size_t i=0;i<column_count;++i){
        for(size_t j=0;j<row_count;++j){
            matr_T[i*row_count+j]=matr[j*column_count+i];
        }
    }
    return matr_T;
}
