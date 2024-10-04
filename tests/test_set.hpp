#pragma once

#include "test_task.hpp" // TestFunctionInput class
#include "assert.hpp"  // Assert
#include "../algorithms/scalar_product.hpp"  // scalar products
#include "../common/generators.hpp"  // vector and length generating

AssertionResult test_scalar_product_std_empty_vectors(TestFunctionInput input);
AssertionResult test_scalar_product_simple_empty_vectors(TestFunctionInput input);

AssertionResult test_scalar_product_std_zero_vectors(TestFunctionInput input);
AssertionResult test_scalar_product_simple_zero_vectors(TestFunctionInput input);

AssertionResult test_scalar_product_simple_diffrent_length_of_vectors(TestFunctionInput input);

AssertionResult test_scalar_product_simple_one(TestFunctionInput input);
AssertionResult test_scalar_product_std_one(TestFunctionInput input);

AssertionResult test_scalar_product_universal(TestFunctionInput input);

bool always_throwing_function(int input);

AssertionResult test_assert_any_throw(TestFunctionInput input);
AssertionResult test_assert_throw(TestFunctionInput input);
AssertionResult test_assert_no_throw(TestFunctionInput input);

AssertionResult test_always_failing(TestFunctionInput input);
