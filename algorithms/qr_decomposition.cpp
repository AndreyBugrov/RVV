#include "qr_decomposition.hpp"

#include <iostream>

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    // checks
    if(! perform_QR(matrix, Q_matrix, R_matrix, row_count, column_count)){
        return;
    }
    // vector system initialization
    vector<vector<num_type>> vec_system(column_count);
    for(size_t j=0;j<column_count;++j){
        for(size_t i=0;i<row_count;++i){
            vec_system[j].push_back(matrix[i*column_count+j]);
        }
    }
    // Gram-Schmidt process
    vector<vector<num_type>> orthogonal_system = gram_schmidt_base_simple(vec_system);
    // vector system normalization
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
    matrix_product_base_simple(Q_matrix_transposed, matrix, R_matrix, column_count, row_count, column_count);
    // get Q^T matrix from Q
    Q_matrix = transpose_matrix(Q_matrix_transposed, column_count, row_count);
}

void QR_decomposition_row_product_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    // checks
    if(! perform_QR(matrix, Q_matrix, R_matrix, row_count, column_count)){
        return;
    }
    // vector system initialization
    vector<vector<num_type>> vec_system(column_count);
    for(size_t j=0;j<column_count;++j){
        for(size_t i=0;i<row_count;++i){
            vec_system[j].push_back(matrix[i*column_count+j]);
        }
    }
    // Gram-Schmidt process
    vector<vector<num_type>> orthogonal_system = gram_schmidt_base_simple(vec_system);
    // vector system normalization
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
    matrix_product_row_simple(Q_matrix_transposed, matrix, R_matrix, column_count, row_count, column_count);
    // get Q^T matrix from Q
    Q_matrix = transpose_matrix(Q_matrix_transposed, column_count, row_count);
}

void QR_decomposition_row_product_matrix_process_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    if(! perform_QR(matrix, Q_matrix, R_matrix, row_count, column_count)){
        return;
    }
    vector<num_type> Q_matrix_transposed = transpose_matrix(matrix, row_count, column_count);
    Q_matrix_transposed = gram_schmidt_matrix_simple_inplace(Q_matrix_transposed, column_count, row_count);
    for(size_t vec_num = 0;vec_num<column_count;++vec_num){
        normalize_vector_inplace(&Q_matrix_transposed[vec_num*row_count], get_vector_norm(&Q_matrix_transposed[vec_num*row_count], row_count), row_count);
    }
    matrix_product_row_simple(Q_matrix_transposed, matrix, R_matrix, column_count, row_count, column_count);
    // get Q^T matrix from Q
    Q_matrix = transpose_matrix(Q_matrix_transposed, column_count, row_count);
}

bool perform_QR(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    if(matrix.size() == 0){
        return false;
    }
    if(is_vector_zero(matrix)){
        generate_zero_array(Q_matrix.data(), row_count*column_count);
        generate_zero_array(R_matrix.data(), column_count*column_count);
        return false;
    }
    return true;
}

// void QR_decomposition_householder(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
//     // checks
//     if(matrix.size() == 0){
//         return;
//     }
//     if(is_vector_zero(matrix)){
//         generate_zero_array(Q_matrix.data(), row_count*column_count);
//         generate_zero_array(R_matrix.data(), column_count*column_count);
//         return;
//     }
//     // create result matrix
//     vector<num_type> result_matrix = transpose_matrix(matrix, row_count, column_count);
//     // fill Q matrix
//     generate_identity_matrix(Q_matrix.data(), row_count, column_count);
//     // householder algorithm
//     for(size_t column_index = 0; column_index < column_count; ++column_index){   
//         vector<num_type> householder_vector = {result_matrix.data()+column_index*row_count+column_index, result_matrix.data()+(column_index+1)*row_count};
//         num_type sign = copysign(1.0, householder_vector[0]);
//         num_type square_norm = dot_product_opt(householder_vector, householder_vector, householder_vector.size());
//         num_type extra_addition = sign * sqrt(square_norm);
//         householder_vector[0] += extra_addition;
//         num_type base_element = result_matrix[column_index*row_count+column_index];
//         num_type householder_coefficient = sqrt(2.0 * (square_norm + base_element * extra_addition));
//         for(size_t element_index = 0; element_index < row_count; ++element_index){
//             householder_vector[element_index] /= householder_coefficient;
//         }
//         num_type subtrahend;
//         for(size_t i=0; i < column_count; ++i){
//             subtrahend = 0.0;
//             for(size_t j=0; j < row_count; ++j){
//                 subtrahend += householder_vector[j]*result_matrix[(i+column_index)*row_count+j];
//             }
//             for(size_t j=0; j < row_count; ++j){
//                 result_matrix[(i+column_index)*row_count+j] -= 2 * householder_vector[j] * subtrahend;
//             }
//         }
//         for(size_t i=0; i < row_count; ++i){
//             subtrahend = 0.0;
//             for(size_t j=0; j< column_count; ++j){
//                 // column_index = row_index for identity matrix
//                 subtrahend += householder_vector[i]*Q_matrix[(i+column_index)*row_count+j];
//             }
//             for(size_t j=0; j< column_count; ++j){
//                 result_matrix[(i+column_index)*row_count+j] -= 2 * householder_vector[i] * subtrahend;
//             }
//         }
//     }
//     for(size_t column_index = 0; column_index < column_count; ++column_index){
        
//     }
// }

// vector<num_type> get_householder_vector(num_type* data, size_t size){
//     vector<num_type> householder_vector = {data, data+size};
//     num_type norm = get_vector_norm(householder_vector);
//     num_type signed_norm = copysign(norm, householder_vector[0]);
//     num_type delimiter = householder_vector[0]+signed_norm;
//     householder_vector[0] = 1.0;
//     for(size_t elem_index = 1; elem_index < size; ++elem_index){
//         householder_vector[elem_index] /= delimiter;
//     }
//     return householder_vector;
// }

// vector<num_type> left_housholder_multiplication(vector<num_type>& matrix, const vector<num_type>& householder_vector, size_t matrix_row_count, size_t matrix_column_count, size_t housholder_vector_size){
//     num_type square_norm = dot_product_opt(householder_vector, householder_vector, householder_vector.size());
//     num_type beta = -2.0 * square_norm;
//     vector<num_type> transposed_matrix = transpose_matrix(matrix, matrix_row_count, matrix_column_count);
//     vector<num_type> w = matrix_vector_product_opt(transposed_matrix, householder_vector, matrix_column_count, matrix_row_count, housholder_vector_size);
//     vector<num_type> vector_outer = outer_product_opt(householder_vector, w, housholder_vector_size, w.size());
//     add_vector_inplace(matrix, vector_outer);
// }

// void QR_decomposition_householder_2(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    
// }
