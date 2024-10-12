#pragma once

struct GramSchmidtInput{
    const double* base_matrix;
    double* result_matrix;
    size_t rank;
    size_t column_length;

    GramSchmidtInput(const double* base_matr, double* result_matr, size_t rank_input, size_t column_length_input): 
    base_matrix(base_matr), result_matrix(result_matr), rank(rank_input), column_length(column_length_input){}
    ~GramSchmidtInput() = default;
};
