#pragma once
#include <vector> // vector

#include "../common/generators.hpp" // generators

using std::vector;

void reset_dot_product(const vector<num_type>& a, const vector<num_type>& b, size_t length);
void reset_matrix_product(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);
void reset_gram_schmidt(const vector<vector<num_type>>& vec_system);
void reset_qr(const vector<num_type>& matrix, vector<num_type>& Q_matrix, vector<num_type>& R_matrix, size_t row_num, size_t column_num);