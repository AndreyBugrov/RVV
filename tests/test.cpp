#include "test.hpp"

TestOutput test_scalar_product_simple_empty_vectors(int vector_length){
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    bool passed = 0.0 == scalar_product_simple(a, b);
    return TestOutput(passed);
}

TestOutput test_scalar_product_std_empty_vectors(int vector_length){
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    bool passed = 0.0 == scalar_product_std(a, b);
    return TestOutput(passed);
}