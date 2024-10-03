#include "test_set.hpp"

using std::string;

bool test_scalar_product_std_empty_vectors(TestFunctionInput input){
    size_t vector_length = 0;
    vector<double> a(vector_length), b(vector_length);
    return assert::assert_eq(0.0, scalar_product_std(a, b));
}
bool test_scalar_product_simple_empty_vectors(TestFunctionInput input){
    size_t vector_length = 0;
    vector<double> a(vector_length), b(vector_length);
    return assert::assert_eq(0.0, scalar_product_std(a, b));
}

bool test_scalar_product_simple_zero_vectors(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    return assert::assert_eq(0.0, scalar_product_simple(a, b));
}
bool test_scalar_product_std_zero_vectors(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    return assert::assert_eq(0.0, scalar_product_std(a, b));
}

bool test_scalar_product_simple_one(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 1.0), b(vector_length, 1.0);
    return assert::assert_eq(double(vector_length), scalar_product_simple(a, b));
}
bool test_scalar_product_std_one(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 1.0), b(vector_length, 1.0);
    return assert::assert_eq(double(vector_length), scalar_product_std(a, b));
}

bool test_scalar_product_universal(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length), b(vector_length);
    generate_rand_array(a.data(), vector_length, input.min_value, input.max_value);
    generate_rand_array(b.data(), vector_length, input.min_value, input.max_value);
    return assert::assert_eq(true, scalar_product_std(a, b) == scalar_product_simple(a, b));
}

bool test_assert_any_throw(TestFunctionInput input){
    try{
        assert::assert_any_throw(assert::assert_near<double>, 5.0, 2.0*2.0, 0.1);
    }
    catch(...){
        return assert::assert_false(true);
    }
    return assert::assert_true(true);
}

bool test_assert_throw(TestFunctionInput input){
    try{
        assert::assert_throw(assert::assert_near<double>, AssertionError(""), 5.0, 2.0*2.0, 0.1);
    }
    catch(...){
        return assert::assert_false(true);
    }
    return assert::assert_true(true);
}

bool test_assert_no_throw(TestFunctionInput input){
    try{
        assert::assert_no_throw(assert::assert_near<double>, 4.0, 2.0*2.0, 0.1);
    }
    catch(...){
        return assert::assert_false(true);
    }
    return assert::assert_true(true);
}