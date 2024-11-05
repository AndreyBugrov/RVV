#pragma once

#include "test_task.hpp" // TestFunctionInput class
#include "assert.hpp"  // Assert
#include "../common/generators.hpp"  // vector and length generating
#include "../algorithms/vector_opeations.hpp"  // vector norm
#include "../algorithms/scalar_product.hpp"  // scalar products
#include "../algorithms/matrix_product.hpp"  // matrix products
#include "../algorithms/gram_schmidt.hpp"  // Gram-Schmidt process

#include <iostream> //debug

AssertionResult test_scalar_product_simple_different_length_of_vectors(TestFunctionInput input);

AssertionResult test_scalar_prod(TestFunctionInputExtended input);
AssertionResult test_vector_norm(TestFunctionInputExtended input);
AssertionResult test_matrix_prod(TestFunctionInputExtended input);
AssertionResult test_qram_schmidt(TestFunctionInputExtended input);

AssertionResult test_normalize_vector(TestFunctionInputExtended input);
