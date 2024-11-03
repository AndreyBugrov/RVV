#pragma once

#include "gram_schmidt.hpp"  // Gram-Schmidt process
#include "matrix_product.hpp" // matrix product

using std::vector;

struct QRInput{
    const double* base_matrix;
    double* Q_matrix;
    double* R_matrix;
    size_t base_matrix_row_num;
    size_t base_matrix_column_num;

    QRInput(const double* base_matr, double* Q_matrix_input, double* R_matrix_input, size_t row_num, size_t column_num): 
    base_matrix(base_matr), Q_matrix(Q_matrix_input), R_matrix(R_matrix_input), base_matrix_row_num(row_num), base_matrix_column_num(column_num)
    {}
    ~QRInput() = default;
};

void QR_decomposition_base_simple(const vector<num_type>& matrix, vector<num_type>& Q_matrix_transposed, vector<num_type>& R_matrix, size_t row_num, size_t column_num);
