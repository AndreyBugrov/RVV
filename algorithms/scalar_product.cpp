#include "scalar_product.hpp"

static inline void check_length(size_t a_size, size_t b_size, size_t length){
    if(a_size!=b_size){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Not equal lengths: ", a_size, " and ", b_size));
    }
    if(a_size!=length){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Vector lengths (", a_size, ") are not equal to given length (", length, ")"));
    }
}

double scalar_product_simple(const VectorProdInput& input){
    check_length(input.a.size(), input.b.size(), input.length);
    double prod = 0.0;
    const double* a = input.a.data();
    const double* b = input.b.data();
    for(int i =0; i < input.length; ++i){
        prod += a[i]*b[i];
    }
    return prod;
}

double scalar_product_std(const VectorProdInput& input){
    check_length(input.a.size(), input.b.size(), input.length);
    return std::inner_product(input.a.begin(), input.a.end(), input.b.begin(), 0.);
}

double scalar_product_std(vector<double> a, vector<double> b, size_t length){
    check_length(a.size(), b.size(), length);
    return std::inner_product(a.begin(), a.end(), b.begin(), 0.);
}

double scalar_product_std_unsafe(vector<double> a, vector<double> b, size_t length){
    return std::inner_product(a.begin(), a.end(), b.begin(), 0.);
}

double scalar_product_boost(const VectorProdInput& input){
    check_length(input.a.size(), input.b.size(), input.length);
    return 0.;
}