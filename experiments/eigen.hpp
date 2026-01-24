#pragma once

#include "../common/defines.hpp"
#include <Eigen/Dense>

void eigen_qr_decomposition(const vector_num& matrix, vector_num& Q_matrix, vector_num& R_matrix, size_t row_count, size_t column_count);
void copy_matrix_to_eigen_matrix(const vector_num& matrix, Eigen::Matrix<num_type, Eigen::Dynamic, Eigen::Dynamic>& eigen_matrix, size_t row_count, size_t column_count);