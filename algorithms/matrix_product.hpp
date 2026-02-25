#pragma once

#include <vector>  // matrix representation

#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string, num_type
#include "vector_operations.hpp" // inner_multiply_vector_by_number_unrolling, OpenMP

using std::vector;

const size_t kBlockSize = 100;

void matrix_product_base_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count);

void matrix_product_row_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count);
void matrix_product_row_scalar(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count);

void matrix_product_row_block(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count);
void matrix_product_row_block_scalar(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count);
