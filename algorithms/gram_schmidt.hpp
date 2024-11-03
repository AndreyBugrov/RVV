#pragma once

#include <cstddef>  // size_t
#include <vector>  // matrices

#include "vector_opeations.hpp"  // vector multiplication from scalar product, mult by number, inplace vector minus

using std::vector;

struct GramSchmidtInput{
    const double* base_matrix;
    double* result_matrix;
    size_t rank;
    size_t column_length;

    GramSchmidtInput(const double* base_matr, double* result_matr, size_t rank_input, size_t column_length_input): 
    base_matrix(base_matr), result_matrix(result_matr), rank(rank_input), column_length(column_length_input){}
    ~GramSchmidtInput() = default;
};

vector<num_type> proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec);

vector<vector<num_type>> gram_schmidt_base_simple(vector<vector<num_type>>& vec_system);
