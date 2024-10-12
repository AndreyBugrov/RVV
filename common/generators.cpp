#include "generators.hpp"

void generate_rand_array(double* arr, size_t n, double min, double max){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<double> gen(min, max);
    for(int i=0;i<n;i++){
        arr[i] = gen(engine);
    }
}

int generate_rand_number(int min, size_t max){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> gen(min, max);
    return gen(engine);
}

void generate_zero_array(double* arr, size_t n){
    memset(arr, 0, sizeof(double)*n);
}

void generate_identity_matrix(double* matrix, size_t row_num, size_t column_num){
    size_t diag_length = row_num;
    if(row_num > column_num){
        diag_length = column_num;
    }
    // no zero generation because of default generation
    for(size_t i=0;i<diag_length;++i){
        matrix[i*column_num+i]=1.0;
    }
}
