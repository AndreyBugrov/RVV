#pragma once

#include <vector> // std::vector as math vector
#include <numeric> // std::inner_product
#include <asm/unistd.h> // intrinsics

#include "omp.h" // openmp

#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string, defines.hpp

using std::vector;

num_type dot_product_simple(const vector<num_type>& a, const vector<num_type>& b, size_t length);
num_type dot_product_std(const vector<num_type>& a, const vector<num_type>& b, size_t length);
num_type dot_product_simple_unsafe(const vector<num_type>& a, const vector<num_type>& b, size_t length);
num_type dot_product_simd(const vector<num_type>& a, const vector<num_type>& b, size_t length);
num_type dot_product_unrolling(const vector<num_type>& a, const vector<num_type>& b, size_t length);

num_type inner_simple_dot_product(const num_type* a, const num_type* b, size_t length);
num_type inner_dot_product_simd(const num_type* a, const num_type* b, size_t length);
num_type inner_dot_product_unrolling(const num_type* a, const num_type* b, size_t length);
