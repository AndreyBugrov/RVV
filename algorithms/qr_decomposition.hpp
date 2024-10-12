#pragma once

#include <cstddef>  // size_t

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
