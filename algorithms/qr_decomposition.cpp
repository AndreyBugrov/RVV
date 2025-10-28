#include "qr_decomposition.hpp"

#include <iostream>

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_non_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, matrix_product_base_simple);
}

void QR_decomposition_row_product_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_non_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, matrix_product_row_simple);
}

void QR_decomposition_row_product_matrix_process_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, gram_schmidt_matrix_simple, matrix_product_row_simple);
}

void QR_decomposition_simd(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, gram_schmidt_matrix_simd, matrix_product_row_simple);
}

void QR_decomposition_unrolling(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){  
    QR_decomposition_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, gram_schmidt_matrix_unrolling, matrix_product_row_simple);
}

void QR_decomposition_double_unrolling(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, gram_schmidt_matrix_unrolling, matrix_product_row_scalar);
}

void QR_decomposition_block(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, gram_schmidt_matrix_unrolling, matrix_product_row_block);
}

void QR_decomposition_block_scalar(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, gram_schmidt_matrix_unrolling, matrix_product_row_block_scalar);
}

void QR_decomposition_block_scalar_inline(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, gram_schmidt_matrix_inline, matrix_product_row_block_scalar);
}

void QR_decomposition_full_matrix(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    QR_decomposition_matrix_common(matrix, Q_matrix, R_matrix, row_count, column_count, gram_schmidt_full_matrix, matrix_product_row_block_scalar);
}

void QR_decomposition_non_matrix_common(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count, matrix_product_function matrix_foo){
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
    matrix_foo(Q_matrix_transposed, matrix, R_matrix, column_count, row_count, column_count);
    // get Q^T matrix from Q
    Q_matrix = transpose_matrix(Q_matrix_transposed, column_count, row_count);
}

void QR_decomposition_matrix_common(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count, orthogonalization_function gs_process, matrix_product_function matrix_foo){
    if(! perform_QR(matrix, Q_matrix, R_matrix, row_count, column_count)){
        return;
    }
    vector<num_type> Q_matrix_transposed = transpose_matrix(matrix, row_count, column_count);
    Q_matrix_transposed = gs_process(Q_matrix_transposed, column_count, row_count);
    for(size_t vec_num = 0;vec_num<column_count;++vec_num){
        normalize_vector_inplace(&Q_matrix_transposed[vec_num*row_count], get_vector_norm(&Q_matrix_transposed[vec_num*row_count], row_count), row_count);
    }
    // count R matrix (R = Q^T * A)
    matrix_foo(Q_matrix_transposed, matrix, R_matrix, column_count, row_count, column_count);
    // get Q^T matrix from Q
    Q_matrix = transpose_matrix(Q_matrix_transposed, column_count, row_count);
}

void create_householder_vector(const num_type* x, int n, num_type* householder_vector) {   
    memset(householder_vector, 0, n*sizeof(x[0]));
    householder_vector[0] = get_vector_norm(x, n);

    for (int i = 0; i < n; ++i){
        householder_vector[i] = x[i] - householder_vector[i];
    }

    double householder_norm = get_vector_norm(householder_vector, n);
    for (int i = 0; i < n; ++i){
        householder_vector[i] /= householder_norm;
    }
}


void hhMulRight(const double* u, int k, size_t row_count, size_t column_count, int n, vector_num& Q_matrix, double* tmp) {
    generate_zero_array(tmp, row_count);

    for(size_t i = 0; i < row_count; ++i) {
        for(size_t j = k; j < column_count; ++j) {
            tmp[i] += Q_matrix[i*n + j] * u[j-k];
        }
    }

    for(size_t i = 0; i < row_count; ++i) {
        for(size_t j = k; j < column_count; ++j) {
            Q_matrix[i*n + j] -= 2*tmp[i] * u[j-k];
        }
    }
}
 
void hhMulLeft(const double* u, int k, size_t row_count, size_t column_count, int n, vector_num& R_matrix, double* tmp) {
    generate_zero_array(tmp, column_count);

    for(size_t j = k; j < column_count; ++j) {
        for(size_t i = 0; i < column_count; ++i) {
            tmp[i] += R_matrix[j*n + i] * u[j-k];
        }
    }

    for(size_t j = k; j < column_count; ++j) {
        for(size_t i = 0; i < column_count; ++i) {
            R_matrix[j*n + i] -= 2*tmp[i] * u[j-k];
        }
    }
}

void QR_decomposition_base_householder(const std::vector<num_type>& matrix,
    std::vector<num_type>& Q_matrix,
    std::vector<num_type>& R_matrix,
    size_t row_count,
    size_t column_count) {
    if(! perform_QR(matrix, Q_matrix, R_matrix, row_count, column_count)){
        return;
    }
    int n = std::min(row_count, column_count);
    R_matrix = matrix;

    generate_identity_matrix(Q_matrix.data(), row_count, column_count);

    //qr
    num_type vec_i[n] = {0};   
    num_type householder_vector[n] = {0};
    num_type tmp[std::max(row_count, column_count)] = {0};

    for(int i = 0; i < n - 1; ++i){
        for(int j = i; j < n; ++j){
            vec_i[j - i] = R_matrix[j*n + i];
        }
        create_householder_vector(vec_i, n - i, householder_vector);
        hhMulLeft(householder_vector, i, row_count, column_count, n, R_matrix, tmp);
        hhMulRight(householder_vector, i, row_count, column_count, n, Q_matrix, tmp);
    }
}

bool perform_QR(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
    if(matrix.size() == 0){
        return false;
    }
    if(row_count < column_count){
        throw Exception(ErrorType::kIncorrectLengthRatio, generate_string("Matrix column count (", column_count, ") is more than matrix row count (", row_count, ")"));
    }
    if(matrix.size() != row_count * column_count){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Base matrix size (", matrix.size(), ") is not equal to given length: ", row_count, " * ", column_count, ")"));
    }
    if(Q_matrix.size() != row_count * column_count){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Q matrix size (", Q_matrix.size(), ") is not equal to given length: ", row_count, " * ", column_count, ")"));
    }
    if(R_matrix.size() != column_count * column_count){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("R matrix size (", R_matrix.size(), ") is not equal to given length: ", column_count, " * ", column_count, ")"));
    }
    if(is_vector_zero(matrix)){
        generate_zero_array(Q_matrix.data(), row_count*column_count);
        generate_zero_array(R_matrix.data(), column_count*column_count);
        return false;
    }
    return true;
}
