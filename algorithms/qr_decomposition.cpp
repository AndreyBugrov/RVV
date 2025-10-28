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

// void tmpQR(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
//     if(! perform_QR(matrix, Q_matrix, R_matrix, row_count, column_count)){
//         return;
//     }
//     // deepseek2 algorithm version
//     vector_num QR = transpose_matrix(matrix, row_count, column_count);
//     vector_num tau(std::min(row_count, column_count));
//     for (size_t base_column_index = 0; base_column_index < column_count; ++base_column_index) {
//         const num_type diagonal_index = base_column_index * row_count + base_column_index;
//         num_type norm = get_vector_norm(&QR[diagonal_index], row_count - base_column_index);
//         if (norm < kAlmostZero) {
//             tau[base_column_index] = 0.0;
//             continue;
//         }
//         num_type sign = (QR[diagonal_index] >= 0) ? 1.0 : -1.0;
//         // Сохраняем коэффициент
//         tau[base_column_index] = 2.0 / (first_householder_vector_element * first_householder_vector_element + norm * norm - QR[diagonal_index] * QR[diagonal_index]);
        
//         // Нормализуем первый элемент
//         QR[diagonal_index] = -sign * norm;
            
//         // Применяем преобразование к оставшимся столбцам
//         for (size_t next_column_index = base_column_index + 1; next_column_index < column_count; ++next_column_index) {
//             num_type dot = first_householder_vector_element * QR[next_column_index * row_count + base_column_index];
//             for (size_t i = base_column_index + 1; i < row_count; i++) {
//                 dot += QR[base_column_index * row_count + i] * QR[next_column_index * row_count + i];
//             }
//             dot *= tau[base_column_index];
//             QR[next_column_index * row_count + base_column_index] -= first_householder_vector_element * dot;
//             for (size_t i = base_column_index + 1; i < row_count; ++i) {
//                 QR[next_column_index * row_count + i] -= QR[base_column_index * row_count + i] * dot;
//             }
//         }
//     }

//     R_matrix = transpose_matrix(QR, column_count, row_count);
//     for (size_t column_index; column_index < column_count; ++column_index){
//         R_matrix[column_index * row_count + column_index] *= (-1);
//     }
// }

// void QR_decomposition_base_base_householder(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count){
//     if(! perform_QR(matrix, Q_matrix, R_matrix, row_count, column_count)){
//         return;
//     }
//     // deepseek2 algorithm version
//     vector_num QR = transpose_matrix(matrix, row_count, column_count);
//     num_type* householder_vectors = new num_type[row_count * column_count];
//     for (size_t base_column_index = 0; base_column_index < column_count; ++base_column_index) {
//         const num_type diagonal_index = base_column_index * row_count + base_column_index;
//         const num_type vector_size = row_count - base_column_index;
//         const num_type vector_end = diagonal_index + vector_size;
//         num_type norm = get_vector_norm(&QR[diagonal_index], vector_size);
//         if (norm < kAlmostZero) {
//             continue;
//         }
//         num_type sign = (QR[diagonal_index] >= 0) ? 1.0 : -1.0;

//         householder_vectors[diagonal_index] = QR[diagonal_index] + sign * norm;
//         for(size_t i = diagonal_index + 1; i < vector_end; ++i){
//             householder_vectors[i] = QR[i];
//         }
//         num_type householder_vector_norm = get_vector_norm(householder_vectors, vector_size);
//         for(size_t i = diagonal_index; i < vector_end; ++i){
//             householder_vectors[i] /= householder_vector_norm;
//         }

//         // Применение преобразования только к R
//         for (int j = k; j < column_count; j++) {
//             double dot_product = 0.0;
//             for (int i = k; i < row_count; i++) {
//                 dot_product += householder_vector[i - k] * flat_at(R_flat, n, i, j);
//             }
            
//             for (int i = k; i < m; i++) {
//                 flat_at(R_flat, n, i, j) -= 2.0 * householder_vector[i - k] * dot_product;
//             }
//         }

//         for (size_t next_column_index = base_column_index; next_column_index < column_count; ++next_column_index) {
//             num_type dot_product = 0.0;
//             dot_product = dot_product_simple_unsafe(&householder_vectors[diagonal_index], &QR[diagonal_index], vector_size);
//             for (size_t i = base_column_index; i < row_count; ++i) {
//                 QR[next_column_index * row_count + i] -= 2.0 * householder_vectors[diagonal_index] * dot_product;
//             }
//         }
//     }
//     R_matrix = transpose_matrix(QR, column_count, row_count);


//     Q_matrix = generate_identity_matrix(column_count, column_count)
        
//         // Применяем преобразования Хаусхолдера в обратном порядке
//         for (int k = std::min(m, n) - 1; k >= 0; k--) {
//             if (householder_vectors[k].empty()) continue;
            
//             const std::vector<double>& v = householder_vectors[k];
            
//             for (int j = 0; j < m; j++) {
//                 double dot_product = 0.0;
//                 for (int i = k; i < m; i++) {
//                     dot_product += v[i - k] * Q_flat[i * m + j];
//                 }
                
//                 for (int i = k; i < m; i++) {
//                     Q_flat[i * m + j] -= 2.0 * v[i - k] * dot_product;
//                 }
//             }
//         }
        
//         return Q_flat;
// }

void bad_QR_decomposition_base_householder(
    const std::vector<num_type>& matrix,
    std::vector<num_type>& Q_matrix,
    std::vector<num_type>& R_matrix,
    size_t row_count,
    size_t column_count){
    if(! perform_QR(matrix, Q_matrix, R_matrix, row_count, column_count)){
        return;
    }
    // Транспонируем исходную матрицу для эффективного доступа к столбцам
    std::vector<num_type> matrix_transposed = transpose_matrix(matrix, row_count, column_count);
    
    // Инициализация рабочих матриц (также в транспонированном виде)
    std::vector<num_type> R_transposed(column_count * row_count, 0.0);
    std::vector<num_type> Q_transposed(column_count * row_count, 0.0);
    
    // Копируем транспонированную матрицу в R_transposed
    R_transposed = matrix_transposed;
    
    // Инициализируем Q_transposed как единичную матрицу (в транспонированном виде)
    generate_identity_matrix(Q_transposed.data(), column_count, row_count);
    // for (size_t col_idx = 0; col_idx < row_count; ++col_idx) {
    //     for (size_t row_idx = 0; row_idx < row_count; ++row_idx) {
    //         Q_transposed[col_idx * row_count + row_idx] = (row_idx == col_idx) ? 1.0 : 0.0;
    //     }
    // }
    
    // Временные векторы для вычислений
    std::vector<num_type> householder_vector(row_count);
    std::vector<num_type> temp_vector(column_count);
    
    // Основной цикл по строкам (в транспонированном представлении строки = исходные столбцы)
    size_t iteration_count = std::min(row_count, column_count);
    for (size_t row_idx = 0; row_idx < iteration_count; ++row_idx) {
        // Указатель на текущую строку в R_transposed
        num_type* current_row = &R_transposed[row_idx * row_count];
        
        // Вычисление нормы текущей строки (начиная с диагонального элемента)
        num_type row_norm = get_vector_norm(current_row + row_idx, row_count - row_idx);
        
        if (std::abs(row_norm) < kAlmostZero) {
            continue; // Нулевая строка - пропускаем преобразование
        }
        
        // Формирование вектора Хаусхолдера
        num_type diagonal_element = current_row[row_idx];
        num_type sign_value = (diagonal_element >= 0) ? 1.0 : -1.0;
        num_type first_element = diagonal_element + sign_value * row_norm;
        
        // Инициализируем вектор Хаусхолдера
        householder_vector[row_idx] = first_element;
        for (size_t col_idx = row_idx + 1; col_idx < row_count; ++col_idx) {
            householder_vector[col_idx] = current_row[col_idx];
        }
        
        // Норма вектора Хаусхолдера
        num_type householder_norm = get_vector_norm(householder_vector.data() + row_idx, row_count - row_idx);
        
        if (std::abs(householder_norm) < kAlmostZero) {
            continue;
        }
        
        // Нормализация вектора Хаусхолдера
        for (size_t col_idx = row_idx; col_idx < row_count; ++col_idx) {
            householder_vector[col_idx] /= householder_norm;
        }
        
        // Применение преобразования Хаусхолдера к R_transposed
        for (size_t vector_index = row_idx; vector_index < column_count; ++vector_index) {
            num_type* target_row = &R_transposed[vector_index * row_count];
            
            num_type dot_product = 0.0;
            for (size_t element_index = row_idx; element_index < row_count; ++element_index) {
                dot_product += householder_vector[element_index] * target_row[element_index];
            }
            
            for (size_t element_index = row_idx; element_index < row_count; ++element_index) {
                target_row[element_index] -= 2.0 * householder_vector[element_index] * dot_product;
            }
        }
        
        // Применение преобразования Хаусхолдера к Q_transposed
        for (size_t vector_index = 0; vector_index < column_count; ++vector_index) {
            num_type* target_row = &Q_transposed[vector_index * row_count];
            
            num_type dot_product = 0.0;
            for (size_t element_index = row_idx; element_index < row_count; ++element_index) {
                dot_product += householder_vector[element_index] * target_row[element_index];
            }
            
            for (size_t element_index = row_idx; element_index < row_count; ++element_index) {
                target_row[element_index] -= 2.0 * householder_vector[element_index] * dot_product;
            }
        }
    }
    
    // Обеспечение положительности диагональных элементов R_transposed
    for (size_t vector_index = 0; vector_index < iteration_count; ++vector_index) {
        num_type* r_row = &R_transposed[vector_index * row_count];
        if (r_row[vector_index] < 0) {
            // Меняем знак всей строки в R_transposed
            for (size_t r_column_index = 0; r_column_index < row_count - vector_index; ++r_column_index) {
                r_row[r_column_index] = -r_row[r_column_index];
            }
            // Меняем знак соответствующего столбца в Q_transposed
            for (size_t q_row_index = 0; q_row_index < column_count; ++q_row_index) {
                Q_transposed[q_row_index * row_count + vector_index] = -Q_transposed[q_row_index * row_count + vector_index];
            }
        }
    }
    
    // Преобразование результатов обратно в обычный формат
    // R_matrix: column_count × column_count в row-major
    // R_matrix.resize(column_count * column_count);
    for (size_t row_idx = 0; row_idx < column_count; ++row_idx) {
        for (size_t col_idx = 0; col_idx < column_count; ++col_idx) {
            if (row_idx < iteration_count && col_idx < row_count) {
                R_matrix[row_idx * column_count + col_idx] = R_transposed[row_idx * row_count + col_idx];
            } else {
                R_matrix[row_idx * column_count + col_idx] = 0.0;
            }
        }
    }
    
    // Q_matrix: row_count × column_count в row-major
    // Q_matrix.resize(row_count * column_count);
    Q_matrix = transpose_matrix(Q_transposed, column_count, row_count);
    // for (size_t row_idx = 0; row_idx < row_count; ++row_idx) {
    //     for (size_t col_idx = 0; col_idx < column_count; ++col_idx) {
    //         Q_matrix[row_idx * column_count + col_idx] = Q_normal[row_idx * row_count + col_idx];
    //     }
    // }
}


inline void create_householder_vector(const num_type* x, int n, num_type* householder_vector) {   
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


inline void hhMulRight(const double* u, int k, size_t row_count, size_t column_count, int n, vector_num& Q_matrix, double* tmp) {
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
 
inline void hhMulLeft(const double* u, int k, size_t row_count, size_t column_count, int n, vector_num& R_matrix, double* tmp) {
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
