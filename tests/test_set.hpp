#pragma once

#include "test_task.hpp" // TestFunctionInput class
#include "assert.hpp"  // Assert
#include "../common/generators.hpp"  // vector and length generating
#include "../algorithms/scalar_product.hpp"  // scalar products
#include "../algorithms/matrix_product.hpp"  // matrix products

bool always_throwing_function(int input);



AssertionResult test_assert_any_throw(TestFunctionInput input);
AssertionResult test_assert_throw(TestFunctionInput input);
AssertionResult test_assert_no_throw(TestFunctionInput input);

AssertionResult test_always_failing(TestFunctionInput input);

AssertionResult test_scalar_product_std_empty_vectors(TestFunctionInput input);
AssertionResult test_scalar_product_simple_empty_vectors(TestFunctionInput input);

AssertionResult test_scalar_product_std_zero_vectors(TestFunctionInput input);
AssertionResult test_scalar_product_simple_zero_vectors(TestFunctionInput input);

AssertionResult test_scalar_product_simple_different_length_of_vectors(TestFunctionInput input);

AssertionResult test_scalar_product_simple_one(TestFunctionInput input);
AssertionResult test_scalar_product_std_one(TestFunctionInput input);

AssertionResult test_scalar_product_universal(TestFunctionInput input);

AssertionResult test_matrix_prod(TestFunctionInputExtended input);
AssertionResult test_matrix_base_simple_prod_identity_matrix(TestFunctionInput input);
