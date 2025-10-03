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
    return dot_product_simple_unsafe(a, b, length);
}

num_type dot_product_simple_unsafe(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    return inner_simple_dot_product(a.data(), b.data(), length);
}

num_type dot_product_std(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    check_length(a.size(), b.size(), length);
    return std::inner_product(a.begin(), a.end(), b.begin(), 0.);
}

num_type dot_product_simd(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    check_length(a.size(), b.size(), length);
    return inner_dot_product_simd(a.data(), b.data(), length);
}

num_type dot_product_unrolling(const vector<num_type>& a, const vector<num_type>& b, size_t length){
    check_length(a.size(), b.size(), length);
    size_t unrolled_data_length = length - length % kUnrollCoefficient;
    num_type unrolled_result = inner_dot_product_unrolling(a.data(), b.data(), unrolled_data_length);
    num_type tail_result = inner_simple_dot_product(a.data()+unrolled_data_length, b.data() + unrolled_data_length, length % kUnrollCoefficient);
    return unrolled_result + tail_result;
}

num_type inner_dot_product_simd(const num_type* a, const num_type* b, size_t length){
    num_type prod = 0.0;
    #pragma omp simd reduction(+:prod)
    for(size_t i = 0; i < length; ++i){
        prod += a[i] * b[i];
    }
    return prod;
}

num_type inner_dot_product_unrolling(const num_type* a, const num_type* b, size_t length){
    num_type prod1 = 0.0, prod2 = 0.0, prod3 = 0.0, prod4 = 0.0;
    for(size_t i = 0; i < length; i += 4){
        prod1 += a[i] * b[i];
        prod2 += a[i+1] * b[i+1];
        prod3 += a[i+2] * b[i+2];
        prod4 += a[i+3] * b[i+3];
    }
    return prod1 + prod2 + prod3 + prod4;
}

num_type inner_simple_dot_product(const num_type* a, const num_type* b, size_t length){
    num_type prod = 0.0;
    for(size_t i = 0; i < length; ++i){
        prod += a[i] * b[i];
    }
    return prod;
}
