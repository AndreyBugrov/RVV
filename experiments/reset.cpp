#include "reset.hpp"

void reset_scalar_product(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    // vectors are unchangable
    return;
}
void reset_matrix_product(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num){
    generate_zero_array(c.data(), a_column_num*b_column_num);
}
void reset_gram_schmidt(const vector<vector<num_type>>& vec_system){
    // vector system is unchangable
    return;
}
void reset_qr(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_num, size_t column_num){
    // unset Q is unnecessary for base qr
    generate_zero_array(Q_matrix.data(), row_num*column_num);
    generate_zero_array(R_matrix.data(), column_num*column_num);
}