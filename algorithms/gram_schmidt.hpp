#pragma once

#include "vector_operations.hpp"  // vector multiplication from scalar product, mult by number, inplace vector minus

using std::vector;

vector<num_type> proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec);

vector<vector<num_type>> gram_schmidt_base_simple(const vector<vector<num_type>>& vec_system);

vector_num gram_schmidt_matrix_simple_inplace(vector_num& transposed_matrix, size_t row_count, size_t column_count);
vector_num gram_schmidt_simd(vector_num& transposed_matrix, size_t row_count, size_t column_count);

vector<vector_num> gram_schmidt_modified_simple(const vector<vector_num>& vec_system);
