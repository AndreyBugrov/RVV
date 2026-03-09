#include <cstring> // std::memcpy
#include <functional> // std::function for QR_decomposition_common

#include "vector_operations.hpp" // get_vector_norm
#include "../common/generators.hpp"  // generate_identity_matrix for QR_decomposition_base_householder, generate_zero_array
#include "dot_product.hpp"

void create_householder_vector(const num_type* x, int n, num_type* householder_vector);

void recalculate_q_matrix(const num_type* u, int k, size_t row_count, size_t column_count, int n, vector_num& Q_matrix, num_type* tmp);
void recalculate_r_matrix(const num_type* u, int k, size_t row_count, size_t column_count, int n, vector_num& R_matrix, num_type* tmp);

void recalculate_q_matrix_unrolling(const num_type* householder_vector, int k, size_t row_count, size_t column_count, int n, vector_num& Q_matrix, num_type* tmp);
void recalculate_r_matrix_unrolling(const num_type* householder_vector, int k, size_t row_count, size_t column_count, int n, vector_num& R_matrix, num_type* tmp);
