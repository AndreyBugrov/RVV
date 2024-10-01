#pragma once

#include "../algorithms/scalar_product.hpp"  // scalar products
#include "../common/common.hpp"  // vector generating
#include "test_input.hpp"  // bring to function test_input

template<class T> bool dumb_test(TestInput<T> input){return true;}

bool test_scalar_product_std_empty_vectors(TestInput<double> input);
bool test_scalar_product_simple_empty_vectors(TestInput<double> input);
