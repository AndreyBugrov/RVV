#pragma once

#include <cstddef>  // size_t
#include <cmath>  // sqrt for norm
#include <vector>  // matrices

#include "scalar_product.hpp"  // vector multiplication

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

num_type get_vector_norm(const vector<num_type>& vec);

void normalize_vector(vector<num_type>& vec, num_type norm);

void sub_vector_from_vector_inplace(const vector<num_type>& minuend, const vector<num_type>& subtrahend);

vector<num_type> proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec);

vector<vector<num_type>> gram_shmidt_base_simple(vector<vector<num_type>>& vec_system);
