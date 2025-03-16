#include "dot_product.hpp"

static void check_length(size_t a_size, size_t b_size, size_t length){
    if(a_size!=b_size){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Not equal lengths: ", a_size, " and ", b_size));
    }
    if(a_size!=length){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Vector lengths (", a_size, ") are not equal to given length (", length, ")"));
    }
}

num_type dot_product_simple(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    check_length(a.size(), b.size(), length);
    num_type prod = 0.0;
    for(size_t i =0; i < length; ++i){
        prod += a[i]*b[i];
    }
    return prod;
}

num_type dot_product_std(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    check_length(a.size(), b.size(), length);
    return std::inner_product(a.begin(), a.end(), b.begin(), 0.);
}

num_type dot_product_opt(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    return inner_optimal_dot_product(a.data(), b.data(), length);
}

num_type dot_product_intrinsic(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    check_length(a.size(), b.size(), length);
    return num_type(0.);
}

num_type inner_optimal_dot_product(const num_type* a, const num_type* b, size_t length){
    num_type prod = 0.0;
    for(size_t i = 0; i < length; ++i){
        prod += a[i] * b[i];
    }
    return prod;
}
