#pragma once

#include <vector>  // matrix representation

#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string, num_type

using std::vector;

void matrix_product_base_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count);

void matrix_product_second_transposed_simple(const vector<num_type>& a, const vector<num_type>& b_T, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count);

void matrix_product_row_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count);
