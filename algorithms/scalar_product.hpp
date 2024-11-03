#pragma once

#include <vector> // std::vector as math vector
#include <numeric> // std::inner_product
#include <asm/unistd.h> // intrinsics

#include "../common/defines.hpp"  // num_type
#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string

using std::vector;

num_type scalar_product_simple(vector<num_type> a, vector<num_type> b, size_t length);
num_type scalar_product_std(vector<num_type> a, vector<num_type> b, size_t length);

num_type scalar_product_std_unsafe(vector<num_type> a, vector<num_type> b, size_t length);

num_type scalar_product_intrinsic(vector<num_type> a, vector<num_type> b, size_t length);
