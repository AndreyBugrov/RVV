#include "scalar_product.hpp"

static void check_length(size_t a_size, size_t b_size, size_t length){
    if(a_size!=b_size){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Not equal lengths: ", a_size, " and ", b_size));
    }
    if(a_size!=length){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Vector lengths (", a_size, ") are not equal to given length (", length, ")"));
    }
}

num_type scalar_product_simple(vector<num_type> a, vector<num_type> b, size_t length){
    check_length(a.size(), b.size(), length);
    double prod = 0.0;
    for(size_t i =0; i < length; ++i){
        prod += a[i]*b[i];
    }
    return prod;
}

num_type scalar_product_std(vector<num_type> a, vector<num_type> b, size_t length){
    check_length(a.size(), b.size(), length);
    return scalar_product_std_unsafe(a, b, length);
}

num_type scalar_product_std_unsafe(vector<num_type> a, vector<num_type> b, size_t length){
    return std::inner_product(a.begin(), a.end(), b.begin(), 0.);
}

num_type scalar_product_intrinsic(vector<num_type> a, vector<num_type> b, size_t length){
    return num_type(0.);
}