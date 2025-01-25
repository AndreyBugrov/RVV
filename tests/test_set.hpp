#pragma once

#include "test_task.hpp" // TestFunctionInput class
#include "expect.hpp"  // expect namespace
#include "../common/generators.hpp"  // vector and length generating
#include "../algorithms/vector_opeations.hpp"  // vector norm
#include "../algorithms/scalar_product.hpp"  // scalar products
#include "../algorithms/matrix_product.hpp"  // matrix products
#include "../algorithms/gram_schmidt.hpp"  // Gram-Schmidt process
#include "../algorithms/qr_decomposition.hpp"  // QR decompostion
#include "../algorithms/matrix_operations.hpp"  // matrix transposition in simple QR decompostion

ExpectationResult test_scalar_prod(TestFunctionInputExtended input);
ExpectationResult test_vector_norm(TestFunctionInputExtended input);
ExpectationResult test_normalize_vector(TestFunctionInputExtended input);
ExpectationResult test_matrix_prod(TestFunctionInputExtended input);
ExpectationResult test_matrix_transposition(TestFunctionInputExtended input);
ExpectationResult test_qram_schmidt(TestFunctionInputExtended input);
ExpectationResult test_qr_decomposition(TestFunctionInputExtended input);
