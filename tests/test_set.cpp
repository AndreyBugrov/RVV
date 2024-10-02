#include "test_set.hpp"

using std::string;

bool test_scalar_product_std_empty_vectors(TestFunctionInput input){
    size_t vector_length = 0;
    vector<double> a(vector_length), b(vector_length);
    return Assert<double>::assert_eq(0.0, scalar_product_std(a, b));
}
bool test_scalar_product_simple_empty_vectors(TestFunctionInput input){
    size_t vector_length = 0;
    vector<double> a(vector_length), b(vector_length);
    return Assert<double>::assert_eq(0.0, scalar_product_std(a, b));
}

bool test_scalar_product_simple_zero_vectors(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    return Assert<double>::assert_eq(0.0, scalar_product_simple(a, b));
}
bool test_scalar_product_std_zero_vectors(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    return Assert<double>::assert_eq(0.0, scalar_product_std(a, b));
}

bool test_scalar_product_simple_one(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 1.0), b(vector_length, 1.0);
    return Assert<double>::assert_eq(vector_length, scalar_product_simple(a, b));
}
bool test_scalar_product_std_one(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 1.0), b(vector_length, 1.0);
    return Assert<double>::assert_eq(vector_length, scalar_product_std(a, b));
}

bool test_scalar_product_universal(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length), b(vector_length);
    generate_rand_array(a.data(), vector_length, input.min_value, input.max_value);
    generate_rand_array(b.data(), vector_length, input.min_value, input.max_value);
    return Assert<bool>::assert_eq(true, scalar_product_std(a, b) == scalar_product_simple(a, b));
}

bool exception_test(TestFunctionInput input){
    try{
        Assert<double>::assert_near(5.0, 2.0*2.0, 0.1);
    }
    catch (AssertionError ass_err){
        return true;
    }
    return Assert<string>::assert_eq("AssertionError", "no exception");
}
