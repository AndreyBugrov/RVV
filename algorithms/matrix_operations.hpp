#pragma once

#include <vector>
#include <cstddef>  // size_t

#include "../common/defines.hpp" // num_type

using std::vector;

vector<num_type> transpose_matrix(const vector<num_type>& matr, size_t row_num, size_t column_num);
void transpose_matrix_in_place(vector<num_type>& matr, size_t row_num, size_t column_num);
