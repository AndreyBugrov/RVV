#pragma once

#include <vector>  // matrices based on vector

// #include "../third_party/boost_1_86_0/boost/numeric/ublas/matrix.hpp"
// #include "../third_party/boost_1_86_0/boost/numeric/ublas/io.hpp"

#include "../common/exception.hpp"  // Exception

using std::vector;

void matrix_prod_base_seq(const vector<double>& a, const vector<double>& b, vector<double>& c, int a_row_length);

void matrix_prod_row_seq(const vector<double>& a, const vector<double>& b, vector<double>& c, int row_length);
