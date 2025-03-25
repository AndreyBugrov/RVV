#pragma once

#include "test_task.hpp" // TestFunctionInput class
#include "expect.hpp"  // expect namespace
#include "../common/generators.hpp"  // vector and length generating
#include "../algorithms/vector_operations.hpp"  // vector norm
#include "../algorithms/dot_product.hpp"  // scalar products
#include "../algorithms/matrix_product.hpp"  // matrix products
#include "../algorithms/gram_schmidt.hpp"  // Gram-Schmidt process
#include "../algorithms/qr_decomposition.hpp"  // QR decompostion
#include "../algorithms/matrix_operations.hpp"  // matrix transposition in simple QR decompostion

ExpectationResult test_dot_product(TestFunctionInputExtended input);
ExpectationResult test_vector_norm(TestFunctionInputExtended input);
ExpectationResult test_normalize_vector(TestFunctionInputExtended input);
ExpectationResult test_matrix_product(TestFunctionInputExtended input);
ExpectationResult test_matrix_transposition(TestFunctionInputExtended input);
ExpectationResult test_gram_schmidt(TestFunctionInputExtended input);
ExpectationResult test_qr_decomposition(TestFunctionInputExtended input);

ExpectationResult test_wrong_dot_product(FunctionOptimizationType function_type, const vector_num& a, const vector_num& b, size_t length);
