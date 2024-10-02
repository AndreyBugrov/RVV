#include "generators.hpp"

void generate_rand_array(double* arr, int n, double min, double max){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<double> gen(min, max);
    for(int i=0;i<n;i++){
        arr[i] = gen(engine);
    }
}

int generate_rand_number(int min, int max){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> gen(min, max);
    return gen(engine);
}

void generate_zero_array(double* arr, int n){
    memset(arr, 0, sizeof(double)*n);
}
