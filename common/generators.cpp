#include "generators.hpp"

void generate_rand_array(num_type* arr, size_t n, num_type min, num_type max){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<num_type> gen(min, max);
    for(size_t i=0;i<n;++i){
        arr[i] = gen(engine);
    }
}

size_t generate_rand_length(size_t min, size_t max){
    std::random_device rd;
    std::mt19937_64 engine(rd());
    std::uniform_int_distribution<size_t> gen(min, max);
    return gen(engine);
}

void generate_zero_array(num_type* arr, size_t n){
    memset(arr, num_type(0), sizeof(num_type)*n);
}

void generate_identity_matrix(num_type* matrix, size_t row_count, size_t column_count){
    size_t diag_length = std::min(row_count, column_count);
    generate_zero_array(matrix, row_count*column_count);
    for(size_t i=0;i<diag_length;++i){
        matrix[i*column_count+i]=num_type(1.0);
    }
}
