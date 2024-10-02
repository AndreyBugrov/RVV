#include "test_set.hpp"

using std::string;

bool test_scalar_product_simple_empty_vectors(TestInput<double> input){
    size_t vector_length = input.length();
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    return 0.0 == scalar_product_simple(a, b);
}

bool test_scalar_product_std_empty_vectors(TestInput<double> input){
    size_t vector_length = input.length();
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    return 0.0 == scalar_product_std(a, b);
}