#pragma once

#include <vector>  // std::vector as representation for mathemacal vectors
#include <cstddef> // size_t

#include "../common/defines.hpp" // num_type

using std::vector;

std::vector<num_type> outer_product_opt(const vector<num_type>& a, const vector<num_type>& b, size_t a_length, size_t b_length);
