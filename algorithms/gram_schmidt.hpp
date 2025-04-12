#pragma once

#include <functional> // std::function for projections
#include "vector_operations.hpp"  // vector multiplication from scalar product, mult by number, inplace vector minus

using std::vector;

vector<num_type> vector_proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec);

void proj(const num_type* projected, const num_type* mapped_vec, num_type* proj, size_t length);
void proj_simd(const num_type* projected, const num_type* mapped_vec, num_type* projection, size_t length);
void proj_unrolling(const num_type* projected, const num_type* mapped_vec, num_type* projection, size_t length);

void vector_matrix_product(const num_type* vec, const num_type* transposed_matrix, num_type* result_vec, size_t row_count, size_t column_count);

using proj_function = std::function<void(const num_type*, const num_type*, num_type*, size_t)>;
using sub_function = std::function<void(num_type*, const num_type*, size_t)>;

vector<vector<num_type>> gram_schmidt_base_simple(const vector<vector<num_type>>& vec_system);

vector_num gram_schmidt_matrix_simple(vector_num& transposed_matrix, size_t row_count, size_t column_count);
vector_num gram_schmidt_matrix_simd(vector_num& transposed_matrix, size_t row_count, size_t column_count);
vector_num gram_schmidt_matrix_unrolling(vector_num& transposed_matrix, size_t row_count, size_t column_count);

vector_num gram_schmidt_matrix_inline(vector_num& transposed_matrix, size_t row_count, size_t column_count);
vector_num gram_schmidt_full_matrix(vector_num& transposed_matrix, size_t row_count, size_t column_count);

vector_num gram_schmidt_matrix_common(vector_num& transposed_matrix, size_t row_count, size_t column_count, proj_function proj_foo, sub_function sub_foo);

void check_matrix(vector_num& transposed_matrix, size_t row_count, size_t column_count);
