#include "matrix_operations.hpp"

vector<num_type> transpose_matrix(const vector<num_type>& matr, size_t row_num, size_t column_num){
    vector<num_type> matr_T(matr);
    transpose_matrix_in_place(matr_T, row_num, column_num);
    return matr_T;
}
void transpose_matrix_in_place(vector<num_type>& matr, size_t row_num, size_t column_num){
    for(size_t i=0;i<row_num;++i){
        for(size_t j=i;j<column_num;++j){
            std::swap(matr[i * row_num + j], matr[j * column_num + i]);
        }
    }
}