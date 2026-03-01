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

void resize_and_generate_matrix(vector_num& matrix, size_t row_count = 0, size_t column_count = 0, AlgebraObjectVersion matrix_version = AlgebraObjectVersion::kZero, num_type min_value = 0., num_type max_value = 0.);
void block_matrix_filling(const TestFunctionInputExtended& input, size_t& a_row_count, size_t& a_column_count, size_t& b_column_count, vector<num_type>& a, vector<num_type>& b, vector<num_type>& c, vector<num_type>& etalon);
bool do_block_matrix_product_immidiately(FunctionOptimizationType function_type, AlgebraObjectVersion object_version);
