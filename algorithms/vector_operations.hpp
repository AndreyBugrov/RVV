#pragma once

#include <cmath>  // sqrt for norm
#include <cstring> // vector copy

#include "omp.h" // simd

#include "dot_product.hpp" // scalar_product_unsafe + std::vector

using std::vector;

num_type get_vector_norm(const vector<num_type>& vec);
num_type get_vector_norm(const num_type* vec, size_t length);
num_type get_vector_norm_simd(const num_type* vec, size_t length);

void normalize_vector_inplace(vector<num_type>& vec, num_type norm);
void normalize_vector_inplace(num_type* vec, num_type norm, size_t length);

bool is_vector_zero(const vector<num_type>& vec);

void sub_vector_from_vector_inplace(vector<num_type>& minuend, const vector<num_type>& subtrahend);
void sub_vector_from_vector_inplace(num_type* minuend, const num_type* subtrahend, size_t length);
void sub_vector_from_vector_inplace_simd(num_type* minuend, const num_type* subtrahend, size_t length);
void add_vector_inplace(vector<num_type>& base_vec, const vector<num_type>& other_vec);

vector<num_type> multiply_vector_by_number(const vector<num_type>& vec, num_type number);
num_type* inner_multiply_vector_by_number(const num_type* vec, num_type number, size_t length);
num_type* inner_multiply_vector_by_number_simd(const num_type* vec, num_type number, size_t length);

void inner_multiply_vector_by_number(const num_type* vec, num_type* mutiplied_vec, num_type number, size_t length);
