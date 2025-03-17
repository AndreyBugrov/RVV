#include <iostream>
#include <Eigen/Dense>
#include <chrono>  // time measurement

#include "algorithms/qr_decomposition.hpp"

#include <cmath>

// from RVV directory:
// g++ -O2 -I ../eigen-3.4.0/ extra_test.cpp algorithms/dot_product.cpp algorithms/gram_schmidt.cpp algorithms/matrix_operations.cpp algorithms/matrix_product.cpp algorithms/qr_decomposition.cpp algorithms/vector_operations.cpp common/exception.cpp common/generators.cpp -o test_eigen.out

const size_t row_num = 2000;
const size_t column_num = 2000;
const num_type kMin = -100.0;
const num_type kMax = 100.0;

using Eigen::MatrixXd;

int main()
{
    // Initialization
    std::vector<num_type> my_matrix(row_num*column_num);
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> eigen_matrix(row_num, column_num);
    generate_rand_array(my_matrix.data(), row_num*column_num, kMin, kMax);
    for(size_t i = 0; i< row_num; ++i){
        for(size_t j =0 ;j < column_num; ++j){
          eigen_matrix(i, j) = my_matrix[i*column_num +j];
        }
    }

    // Mine
    std::vector<num_type> my_q(row_num*column_num);
    std::vector<num_type> my_r(column_num*column_num);
    const auto start_test{std::chrono::steady_clock::now()};
    QR_decomposition_row_product_matrix_process_simple(my_matrix, my_q, my_r, row_num, column_num);
    const auto end_test{std::chrono::steady_clock::now()};
    vector_num my_check_matrix(row_num * column_num);
    const std::chrono::duration<double> test_seconds = end_test - start_test;
    matrix_product_base_simple(my_q, my_r, my_check_matrix, row_num, column_num, column_num);

    // Eigen
    const auto start_householder{std::chrono::steady_clock::now()};
    auto QR = eigen_matrix.householderQr();
    const auto end_householder{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> eigen_seconds = end_householder - start_householder;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> EigenR = QR.matrixQR().triangularView<Eigen::Upper>();
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> EigenQ = QR.householderQ();
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> CheckMatrix = EigenQ * EigenR;
    
    // Checks
    num_type max_eigen_difference = 0.0;
    for(size_t i = 0; i< row_num; ++i){
        for(size_t j =0 ;j < column_num; ++j){
            num_type difference = fabs(eigen_matrix(i, j) - CheckMatrix(i, j));
            if(difference > max_eigen_difference){
                max_eigen_difference = difference;
            }
        }
    }
    num_type max_my_difference = 0.0;
    for(size_t i = 0; i< row_num; ++i){
        for(size_t j =0 ;j < column_num; ++j){
            num_type difference = fabs(my_matrix[i*column_num + j] - my_check_matrix[i*column_num + j]);
            if(difference > max_my_difference){
                max_my_difference = difference;
            }
        }
    }
    std::cout<<"Inaccuracy\n";
    std::cout<<"Eigen Householder inaccuracy: "<<max_eigen_difference<<"\n";
    std::cout<<"My Gram-Schmidt inaccuracy: "<<max_my_difference<<"\n";
    std::cout<<"Time\n";
    std::cout<<"Eigen Householder time: "<<eigen_seconds.count()<<"\n";
    std::cout<<"My Gram-Schmidt time: "<<test_seconds.count()<<"\n";
}