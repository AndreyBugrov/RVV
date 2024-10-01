#pragma once

#include <vector> // std::vector
#include <numeric> // std::inner_product
#include <asm/unistd.h> // architecture 

#include "../common/exception.hpp"

using std::vector;

double scalar_product_simple(const vector<double>& a, const vector<double>& b);

double scalar_product_std(const vector<double>& a, const vector<double>& b);

double scalar_product_intrinsic(const vector<double>& a, const vector<double>& b);
