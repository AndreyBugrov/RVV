#pragma once

#include "gram_schmidt.hpp"  // Gram-Schmidt process, normalize_vector, vector_norm
#include "matrix_product.hpp" // matrix product

using std::vector;

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix_transposed, vector<num_type>& R_matrix, size_t row_num, size_t column_num);
