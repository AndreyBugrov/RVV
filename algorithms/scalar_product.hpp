#pragma once

#include <vector> // std::vector
#include <numeric> // std::inner_product
#include <asm/unistd.h> // architecture 

// #include "../third_party/boost_1_86_0/boost/numeric/ublas/vector.hpp"
// #include "../third_party/boost_1_86_0/boost/numeric/ublas/io.hpp"

#include "../common/exception.hpp"  // Exception

using std::vector;

double scalar_product_simple(const vector<double>& a, const vector<double>& b);

double scalar_product_std(const vector<double>& a, const vector<double>& b);

double scalar_product_boost(const vector<double>& a, const vector<double>& b);

double scalar_product_intrinsic(const vector<double>& a, const vector<double>& b);
