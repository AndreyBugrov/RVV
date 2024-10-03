#pragma once

#include "test_task.hpp" // TestFunctionInput class
#include "assert.hpp"  // Assert
#include "../algorithms/scalar_product.hpp"  // scalar products
#include "../common/generators.hpp"  // vector and length generating

bool test_scalar_product_std_empty_vectors(TestFunctionInput input);
bool test_scalar_product_simple_empty_vectors(TestFunctionInput input);

bool test_scalar_product_std_zero_vectors(TestFunctionInput input);
bool test_scalar_product_simple_zero_vectors(TestFunctionInput input);

bool test_scalar_product_simple_one(TestFunctionInput input);
bool test_scalar_product_std_one(TestFunctionInput input);

bool test_scalar_product_universal(TestFunctionInput input);

bool test_assert_any_throw(TestFunctionInput input);
bool test_assert_throw(TestFunctionInput input);
bool test_assert_no_throw(TestFunctionInput input);

bool always_failing_test(TestFunctionInput input);
