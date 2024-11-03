#pragma once

#include <vector>  // matrices

#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string
#include "scalar_product.hpp"  // scalar products

#include "../common/single_logger.hpp" // logger

using std::vector;

void matrix_prod_base_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);
void matrix_prod_base_std(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);

void matrix_prod_second_transposed_simple(const vector<num_type>& a, const vector<num_type>& b_T, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);

//void matrix_prod_row_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);
// void matrix_prod_row_std(MatrixProdInput& input);
// void matrix_prod_row_intrinsic(MatrixProdInput& input);
