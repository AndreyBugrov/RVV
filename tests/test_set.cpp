#include "test_set.hpp"

#include <iostream>

using std::string;

bool always_throwing_function(int input){
    throw Exception(ErrorType::kUnknownError, "");    
}

AssertionResult test_assert_any_throw(TestFunctionInput input){
    try{
        std::vector<int> a(3);
        assert::assert_any_throw(always_throwing_function, 5);
    }
    catch(...){
        return assert::assert_false(true);
    }
    return assert::assert_true(true);
}

AssertionResult test_assert_throw(TestFunctionInput input){
    try{
        assert::assert_throw(always_throwing_function, Exception(ErrorType::kUnknownError, ""), 5);
    }
    catch(...){
        return assert::assert_false(true);
    }
    return assert::assert_true(true);
}

AssertionResult test_assert_no_throw(TestFunctionInput input){
    try{
        assert::assert_no_throw(assert::assert_near<double>, 4.0, 2.0*2.0, 0.1);
    }
    catch(...){
        return assert::assert_false(true);
    }
    return assert::assert_true(true);
}

AssertionResult test_always_failing(TestFunctionInput input){
    return AssertionResult(false, "Don't pay attention to me");
}

AssertionResult test_scalar_product_std_empty_vectors(TestFunctionInput input){
    size_t vector_length = 0;
    vector<double> a(vector_length), b(vector_length);
    return assert::assert_eq(0.0, scalar_product_std(VectorProdInput(a, b, vector_length)));
}
AssertionResult test_scalar_product_simple_empty_vectors(TestFunctionInput input){
    size_t vector_length = 0;
    vector<double> a(vector_length), b(vector_length);
    return assert::assert_eq(0.0, scalar_product_simple(VectorProdInput(a, b, vector_length)));
}

AssertionResult test_scalar_product_simple_zero_vectors(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    return assert::assert_eq(0.0, scalar_product_simple(VectorProdInput(a, b, vector_length)));
}
AssertionResult test_scalar_product_std_zero_vectors(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 0.0), b(vector_length, 0.0);
    return assert::assert_eq(0.0, scalar_product_std(VectorProdInput(a, b, vector_length)));
}

AssertionResult test_scalar_product_simple_one(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 1.0), b(vector_length, 1.0);
    return assert::assert_eq(double(vector_length), scalar_product_simple(VectorProdInput(a, b, vector_length)));
}
AssertionResult test_scalar_product_std_one(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length, 1.0), b(vector_length, 1.0);
    return assert::assert_eq(double(vector_length), scalar_product_std(VectorProdInput(a, b, vector_length)));
}

AssertionResult test_scalar_product_universal(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length), b(vector_length);
    generate_rand_array(a.data(), vector_length, input.min_value, input.max_value);
    generate_rand_array(b.data(), vector_length, input.min_value, input.max_value);
    VectorProdInput foo_input(a, b, vector_length);
    return assert::assert_eq(true, scalar_product_std(foo_input) == scalar_product_simple(foo_input));
}

AssertionResult test_scalar_product_simple_different_length_of_vectors(TestFunctionInput input){
    size_t vector_length = generate_rand_number(input.min_length, input.max_length)*generate_rand_number(input.min_length, input.max_length);
    vector<double> a(vector_length), b(vector_length+1);
    generate_rand_array(a.data(), vector_length, input.min_value, input.max_value);
    generate_rand_array(b.data(), vector_length, input.min_value, input.max_value);
    return assert::assert_throw(scalar_product_simple, Exception(ErrorType::kUnequalLengthError, ""), VectorProdInput(a, b, vector_length));
}

AssertionResult test_matrix_prod(TestFunctionInputExtended input){
    size_t a_row_number = 0;
    size_t a_column_number = 0;
    size_t b_column_number = 0;
    vector<double> a, b, c;
    vector<double> etalon;

    if(input.verification_type != VerificationType::kEmpty){
        a_row_number = generate_rand_number(input.min_length, input.max_length);
        a_column_number = generate_rand_number(input.min_length, input.max_length);
        b_column_number = generate_rand_number(input.min_length, input.max_length);
        a.resize(a_row_number*a_column_number);
        b.resize(a_column_number*b_column_number);
        c.resize(a_row_number*b_column_number);
        etalon.resize(a_row_number*b_column_number);
    }

    switch (input.verification_type)
    {
    case VerificationType::kEmpty:
        break;
    case VerificationType::kZero:
        break;
    case VerificationType::kIdentity:
        {
            generate_identity_matrix(a.data(), a_row_number, a_column_number);
            std::cout<<"aaaaaaaaaa\n";
            generate_rand_array(b.data(), a_column_number*b_column_number, input.min_value, input.max_value);
            double* a_data = a.data();
            double* b_data = b.data();
            double* etalon_data = etalon.data();
            size_t max_i = std::min(a_row_number, a_column_number);
            size_t max_j = std::min(b_column_number, a_column_number);
            max_j = b_column_number;
            for(size_t i=0;i<max_i;++i){
                for(size_t j=0;j<max_j;++j){
                    etalon_data[i*a_row_number+j] = a_data[i*a_column_number+i]*b_data[i*b_column_number+j];
                }
            }
        }
        break;
    case VerificationType::kRandom:
        generate_rand_array(a.data(), a_row_number*a_column_number, input.min_value, input.max_value);
        generate_rand_array(b.data(), a_column_number*b_column_number, input.min_value, input.max_value);
        break;
    default:
        throw Exception(ErrorType::kValueError, generate_string("Unsupported VerificationType for matrix prod: ", static_cast<int>(input.verification_type)));
        break;
    }
    MatrixProdInput function_input = MatrixProdInput(a, b, c, a_row_number, a_column_number, b_column_number);
    switch (input.function_type)
    {
    case FunctionOptimizationType::kSimple:
        matrix_prod_base_simple(function_input);
        break;
    case FunctionOptimizationType::kSimpleStd:
        //matrix_prod_base_std(function_input);
        break;
    case FunctionOptimizationType::kSimpleIntrinsic:
        //
        break;
    case FunctionOptimizationType::kRow:
        //matrix_prod_row_simple(function_input);
        break;
    case FunctionOptimizationType::kRowStd:
        //matrix_prod_row_std(function_input);
        break;
    case FunctionOptimizationType::kRowIntrinsic:
        //matrix_prod_row_intrinsic(function_input);
        break;
    default:
        break;
    }
    return assert::assert_array_eq(etalon, function_input.c, etalon.size());
}
