#pragma once

#include <cmath>  // sqrt for norm

#include "scalar_product.hpp" // scalar_product_unsafe + std::vector

using std::vector;

num_type get_vector_norm(const vector<num_type>& vec);

void normalize_vector_inplace(vector<num_type>& vec, num_type norm);

void sub_vector_from_vector_inplace(vector<num_type>& minuend, const vector<num_type>& subtrahend);

vector<num_type> multiply_vector_by_number(const vector<num_type>& vec, num_type number);