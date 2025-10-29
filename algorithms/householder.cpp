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

void recalculate_q_matrix(const num_type* householder_vector, int k, size_t row_count, size_t column_count, int n, vector_num& Q_matrix, num_type* tmp) {
    generate_zero_array(tmp, row_count);

    for(size_t row_index = 0; row_index < row_count; ++row_index) {
        for(size_t column_index = k; column_index < column_count; ++column_index) {
            tmp[row_index] += Q_matrix[row_index*column_count + column_index] * householder_vector[column_index-k];
        }
    }

    for(size_t row_index = 0; row_index < row_count; ++row_index) {
        for(size_t column_index = k; column_index < column_count; ++column_index) {
            Q_matrix[row_index*column_count + column_index] -= 2*tmp[row_index] * householder_vector[column_index-k];
        }
    }
}

// void recalculate_q_matrix(const num_type* householder_vector, int k, size_t row_count, 
//                          size_t column_count, int n, vector_num& Q_matrix, num_type* tmp) {
//     // Оптимизация: обнуляем только используемую часть tmp
//     std::fill(tmp, tmp + row_count, num_type(0));

//     // Оптимизация: улучшаем локальность данных
//     const size_t start_col = k;
//     const size_t end_col = column_count;
//     // const int hh_size = n - k;
    
//     for (size_t row_index = 0; row_index < row_count; ++row_index) {
//         num_type* Q_row = &Q_matrix[row_index * column_count];
//         num_type sum = num_type(0);
        
//         // Внутренний цикл с лучшей локальностью
//         for (size_t col_index = start_col; col_index < end_col; ++col_index) {
//             sum += Q_row[col_index] * householder_vector[col_index - start_col];
//         }
//         tmp[row_index] = sum;
//     }

//     // Оптимизация: объединяем вычисления
//     for (size_t row_index = 0; row_index < row_count; ++row_index) {
//         num_type* Q_row = &Q_matrix[row_index * column_count];
//         const num_type factor = num_type(2) * tmp[row_index];
        
//         for (size_t col_index = start_col; col_index < end_col; ++col_index) {
//             Q_row[col_index] -= factor * householder_vector[col_index - start_col];
//         }
//     }
// }

void recalculate_r_matrix(const num_type* householder_vector, int k, size_t row_count, size_t column_count, int n, vector_num& R_matrix, num_type* tmp) {
    generate_zero_array(tmp, column_count);

    for(size_t row_index = k; row_index < row_count; ++row_index) {
        for(size_t column_index = 0; column_index < column_count; ++column_index) {
            tmp[column_index] += R_matrix[row_index*column_count + column_index] * householder_vector[row_index-k];
        }
    }

    for(size_t row_index = k; row_index < row_count; ++row_index) {
        for(size_t column_index = 0; column_index < column_count; ++column_index) {
            R_matrix[row_index*column_count + column_index] -= 2*tmp[column_index] * householder_vector[row_index-k];
        }
    }
}

// void recalculate_r_matrix(const num_type* householder_vector, int k, size_t row_count, 
//                          size_t column_count, int n, vector_num& R_matrix, num_type* tmp) {
//     std::fill(tmp, tmp + column_count, num_type(0));

//     const size_t start_row = k;
//     const size_t end_row = row_count;
//     // const int hh_size = n - k;
    
//     // Оптимизация: транспонированный доступ для лучшей производительности
//     for (size_t col_index = 0; col_index < column_count; ++col_index) {
//         num_type sum = num_type(0);
//         for (size_t row_index = start_row; row_index < end_row; ++row_index) {
//             sum += R_matrix[row_index * column_count + col_index] * 
//                    householder_vector[row_index - start_row];
//         }
//         tmp[col_index] = sum;
//     }

//     for (size_t col_index = 0; col_index < column_count; ++col_index) {
//         const num_type factor = num_type(2) * tmp[col_index];
//         for (size_t row_index = start_row; row_index < end_row; ++row_index) {
//             R_matrix[row_index * column_count + col_index] -= 
//                 factor * householder_vector[row_index - start_row];
//         }
//     }
// }