#pragma once

#include <vector>  // matrices

#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string
#include "scalar_product.hpp"  // scalar products

using std::vector;

struct MatrixProdInput{
    vector<double> a;
    vector<double> b;
    vector<double> c;
    size_t a_row_num;
    size_t a_column_num;
    size_t b_column_num;

    MatrixProdInput(const vector<double>& matr_1, const vector<double>& matr_2, const vector<double>& result_matr, size_t a_row_num_input, size_t a_column_num_input, size_t b_column_num_input): 
    a(matr_1), b(matr_2), c(result_matr), a_row_num(a_row_num_input), a_column_num(a_column_num_input), b_column_num(b_column_num_input){}
    MatrixProdInput(const MatrixProdInput& other): a(other.a), b(other.b), c(other.c), a_row_num(other.a_row_num), a_column_num(other.a_column_num), b_column_num(other.b_column_num)
    {}
    ~MatrixProdInput() = default;
};

void matrix_prod_base_simple(MatrixProdInput& input);
void matrix_prod_base_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);
void matrix_prod_base_std(MatrixProdInput& input);

void matrix_prod_second_transposed_simple(const vector<num_type>& a, const vector<num_type>& b_T, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num);

void matrix_prod_row_simple(MatrixProdInput& input);
// void matrix_prod_row_std(MatrixProdInput& input);
// void matrix_prod_row_intrinsic(MatrixProdInput& input);
