#pragma once

#include <cstring> // std::memcpy
#include <functional> // std::function for QR_decomposition_common

#include "gram_schmidt.hpp"  // Gram-Schmidt process, normalize_vector, vector_norm
#include "matrix_product.hpp" // matrix product
#include "matrix_operations.hpp"  // transpose matrix
#include "outer_product.hpp" // outer product
#include "householder.hpp" // householder operations

using std::vector;

const num_type kAlmostZero = 1e-12;

using matrix_product_function = std::function<void(const vector_num&, const vector_num&, vector_num&, size_t, size_t, size_t)>;
using orthogonalization_function = std::function<vector_num(vector_num&, size_t, size_t)>;

using recalculate_matrix_function = std::function<void(const num_type*, int, size_t, size_t, int, vector_num&, num_type*)>;

using recalculate_matrix_function = std::function<void(const num_type*, int, size_t, size_t, int, vector_num&, num_type*)>;

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_row_product_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);

void QR_decomposition_row_product_matrix_process_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_simd(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_unrolling(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_double_unrolling(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_block(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_block_scalar(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_block_scalar_inline(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_full_matrix(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);

void QR_decomposition_base_householder(const vector_num& matrix, vector_num& Q_matrix, vector_num& R_matrix, size_t row_count, size_t column_count);
void QR_decomposition_householder_unrolling(const vector_num& matrix, vector_num& Q_matrix, vector_num& R_matrix, size_t row_count, size_t column_count);

void QR_decomposition_non_matrix_common(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count, matrix_product_function matrix_foo);
void QR_decomposition_matrix_common(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count, orthogonalization_function gs_process, matrix_product_function matrix_foo);
void QR_decomposition_householder_common(const vector_num& matrix, vector_num& Q_matrix, vector_num& R_matrix, size_t row_count, size_t column_count, recalculate_matrix_function q_function, recalculate_matrix_function r_function);

bool perform_QR(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_count, size_t column_count);
