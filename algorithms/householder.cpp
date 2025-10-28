#include "householder.hpp"

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

void recalculate_q_matrix(const num_type* u, int k, size_t row_count, size_t column_count, int n, vector_num& Q_matrix, num_type* tmp) {
    generate_zero_array(tmp, row_count);

    for(size_t row_index = 0; row_index < row_count; ++row_index) {
        for(size_t column_index = k; column_index < column_count; ++column_index) {
            tmp[row_index] += Q_matrix[row_index*column_count + column_index] * u[column_index-k];
        }
    }

    for(size_t row_index = 0; row_index < row_count; ++row_index) {
        for(size_t column_index = k; column_index < column_count; ++column_index) {
            Q_matrix[row_index*column_count + column_index] -= 2*tmp[row_index] * u[column_index-k];
        }
    }
}

void recalculate_r_matrix(const num_type* u, int k, size_t row_count, size_t column_count, int n, vector_num& R_matrix, num_type* tmp) {
    generate_zero_array(tmp, column_count);

    for(size_t row_index = k; row_index < row_count; ++row_index) {
        for(size_t column_index = 0; column_index < column_count; ++column_index) {
            tmp[column_index] += R_matrix[row_index*column_count + column_index] * u[row_index-k];
        }
    }

    for(size_t row_index = k; row_index < row_count; ++row_index) {
        for(size_t column_index = 0; column_index < column_count; ++column_index) {
            R_matrix[row_index*column_count + column_index] -= 2*tmp[column_index] * u[row_index-k];
        }
    }
}