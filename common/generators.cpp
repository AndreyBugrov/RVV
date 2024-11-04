#include "generators.hpp"

void generate_rand_array(num_type* arr, size_t n, num_type min, num_type max){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<double> gen(min, max);
    for(size_t i=0;i<n;++i){
        arr[i] = gen(engine);
    }
}

int generate_rand_integer_number(int min, int max){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> gen(min, max);
    return gen(engine);
}

void generate_zero_array(num_type* arr, size_t n){
    memset(arr, num_type(0), sizeof(num_type)*n);
}

void generate_identity_matrix(num_type* matrix, size_t row_num, size_t column_num){
    size_t diag_length = std::min(row_num, column_num);
    generate_zero_array(matrix, row_num*column_num);
    for(size_t i=0;i<diag_length;++i){
        matrix[i*column_num+i]=num_type(1.0);
    }
}
