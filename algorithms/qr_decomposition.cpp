#include "qr_decomposition.hpp"

#include <iostream>

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_num, size_t column_num){
    // checks
    if(matrix.size() == 0){
        return;
    }
    if(is_vector_zero(matrix)){
        generate_zero_array(Q_matrix.data(), row_num*column_num);
        generate_zero_array(R_matrix.data(), column_num*column_num);
        return;
    }
    // vector system initialization
    vector<vector<num_type>> vec_system(column_num);
    for(size_t j=0;j<column_num;++j){
        for(size_t i=0;i<row_num;++i){
            vec_system[j].push_back(matrix[i*column_num+j]);
        }
    }
    // Gram-Schmidt process
    vector<vector<num_type>> orthogonal_system = gram_schmidt_base_simple(vec_system);
    // vector system nomalization
    const size_t vec_system_size = orthogonal_system.size();
    for(size_t vec_num = 0;vec_num<vec_system_size;++vec_num){
        normalize_vector_inplace(orthogonal_system[vec_num], get_vector_norm(orthogonal_system[vec_num]));
    }
    // init Q^T matrix
    const size_t vec_system_vec_length = orthogonal_system[0].size();
    vector<num_type> Q_matrix_transposed(vec_system_size*vec_system_vec_length);
    for(size_t vec_num = 0;vec_num<vec_system_size;++vec_num){
        std::memcpy(Q_matrix_transposed.data()+vec_num*vec_system_vec_length, orthogonal_system[vec_num].data(), vec_system_vec_length*sizeof(num_type));
    }
    // count R matrix (R = Q^T * A)
    matrix_prod_base_simple(Q_matrix_transposed, matrix, R_matrix, column_num, row_num, column_num);
    // get Q^T matrix from Q
    Q_matrix = transpose_matrix(Q_matrix_transposed, column_num, row_num);
}