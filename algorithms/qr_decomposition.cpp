#include "qr_decomposition.hpp"

#include <iostream>

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix_transposed, vector<num_type>& R_matrix, size_t row_num, size_t column_num){
    // initialization
    vector<vector<num_type>> vec_system(column_num);
    for(size_t j=0;j<column_num;++j){
        for(size_t i=0;i<row_num;++i){
            vec_system[j].push_back(matrix[i*column_num+j]);
        }
    }
    // gram_schmidt process
    vector<vector<num_type>> orthogonal_system = gram_schmidt_base_simple(vec_system);
    for(size_t vec_num =0;vec_num<orthogonal_system.size();++vec_num){
        normalize_vector_inplace(orthogonal_system[vec_num], get_vector_norm(orthogonal_system[vec_num]));
    }
    // Q_Transposed initialization
    size_t vec_system_size = orthogonal_system.size();
    size_t vec_system_vec_length = orthogonal_system[0].size();
    for(size_t vec_num =0;vec_num<vec_system_size;++vec_num){
        for(size_t coord_num = 0; coord_num<vec_system_vec_length;++coord_num){
            Q_matrix_transposed[vec_num*vec_system_vec_length+coord_num] = orthogonal_system[vec_num][coord_num];
        }
    }
    //////////////////////// should we transpose Q_matrix? It can be better for optimization do not do it
    //matrix product
    // R = Q^T * A => R = base_prod(Q_Transposed, A)
    matrix_prod_base_simple(Q_matrix_transposed, matrix, R_matrix, column_num, row_num, column_num);
    // for(size_t i=0;i<row_num;++i){
    //         std::cout<<Q_matrix_transposed[i*column_num];
    // }
}