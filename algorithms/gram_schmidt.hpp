#pragma once

#include "vector_operations.hpp"  // vector multiplication from scalar product, mult by number, inplace vector minus

using std::vector;

vector<num_type> proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec);
num_type* proj(const num_type* projected, const num_type* mapped_vec, size_t length);
num_type* proj_simd(const num_type* projected, const num_type* mapped_vec, size_t length);
num_type* proj_unrolling(const num_type* projected, const num_type* mapped_vec, size_t length);

vector<vector<num_type>> gram_schmidt_base_simple(const vector<vector<num_type>>& vec_system);

vector_num gram_schmidt_matrix_simple(vector_num& transposed_matrix, size_t row_count, size_t column_count);
vector_num gram_schmidt_matrix_simd(vector_num& transposed_matrix, size_t row_count, size_t column_count);
vector_num gram_schmidt_matrix_unrolling(vector_num& transposed_matrix, size_t row_count, size_t column_count);

void check_matrix(vector_num& transposed_matrix, size_t row_count, size_t column_count);
