#pragma once

#include <cstring> // std::memcpy
#include <functional> // std::function for QR_decomposition_common

#include "gram_schmidt.hpp"  // Gram-Schmidt process, normalize_vector, vector_norm
#include "matrix_product.hpp" // matrix product
#include "matrix_operations.hpp"  // transpose matrix
#include "outer_product.hpp" // outer product


using std::vector;

using matrix_product_function = std::function<void(const vector_num&, const vector_num&, vector_num&, size_t, size_t, size_t)>;
using orthogonalization_function = std::function<vector<vector<num_type>>(const vector<vector_num>&)>;

bool perform_QR(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);

void QR_decomposition_row_product_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);

void QR_decomposition_row_product_matrix_process_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);

void QR_decomposition_simd(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);

void QR_decomposition_common(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count, orthogonalization_function, matrix_product_function);

void QR_decomposition_householder(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_householder_2(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
