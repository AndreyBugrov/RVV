#include "eigen.hpp"

void copy_matrix_to_eigen_matrix(const vector_num& matrix, Eigen::Matrix<num_type, Eigen::Dynamic, Eigen::Dynamic>& eigen_matrix, size_t row_count, size_t column_count){
    for(size_t i = 0; i < row_count; ++i){
        for(size_t j = 0; j < column_count; ++j){
            eigen_matrix(i,j) = matrix[i*column_count + j];
        }
    }
}

void eigen_qr_decomposition(const vector_num& matrix, vector_num& Q_matrix, vector_num& R_matrix, size_t row_count, size_t column_count){
    Eigen::Matrix<num_type, Eigen::Dynamic, Eigen::Dynamic> eigen_matrix(row_count, column_count);
    copy_matrix_to_eigen_matrix(matrix, eigen_matrix, row_count, column_count);
    auto QR = eigen_matrix.householderQr();
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> EigenR = QR.matrixQR().triangularView<Eigen::Upper>();
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> EigenQ = QR.householderQ();
}