#pragma once

#include <vector>  // matrix representation

#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string, num_type

using std::vector;

void matrix_prod_base_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);

void matrix_prod_second_transposed_simple(const vector<num_type>& a, const vector<num_type>& b_T, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);
