#pragma once

#include <cstring> // std::memcpy

#include "gram_schmidt.hpp"  // Gram-Schmidt process, normalize_vector, vector_norm
#include "matrix_product.hpp" // matrix product
#include "matrix_operations.hpp"  // transpose matrix


using std::vector;

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_num, size_t column_num);
