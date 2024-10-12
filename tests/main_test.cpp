#include <iostream>

#include "test_runner.hpp"  // TestRunner class
#include "test_set.hpp"  // list of all tests

int main(){
    std::string function_names[]={
        "matrix product base"
    };
    std::string verification_names[]={
        "(empty",
        "(zero",
        "(identity",
        "(general"
    };
    std::string test_types[]={
        "vectors)",
        "matrices)",
    };

    std::vector<TestTask> tasks{
        // TestTask("always failing", test_always_failing),
        // TestTask("assert any exception", test_assert_any_throw),
        // TestTask("assert no exception", test_assert_no_throw),
        // TestTask("assert specified exception", test_assert_throw),
        // TestTask("scalar product empty vectors", test_scalar_product_simple_empty_vectors),
        // TestTask("inner product empty vectors", test_scalar_product_std_empty_vectors),
        // TestTask("scalar product zero vectors", test_scalar_product_simple_zero_vectors), 
        // TestTask("inner product zero vectors", test_scalar_product_std_zero_vectors),
        // TestTask("scalar product one", test_scalar_product_simple_one),
        // TestTask("inner product one", test_scalar_product_std_one),
        // TestTask("scalar product universal", test_scalar_product_universal),
        // TestTask("throws when lengths of vectors are unequal", test_scalar_product_simple_different_length_of_vectors),
        TestTask(function_names[0]+" "+verification_names[0]+" "+test_types[1], FunctionOptimizationType::kSimple, VerificationType::kEmpty, test_matrix_prod),
        TestTask(function_names[0]+" "+verification_names[1]+" "+test_types[1], FunctionOptimizationType::kSimple, VerificationType::kZero, test_matrix_prod),
        TestTask(function_names[0]+" "+verification_names[2]+" "+test_types[1], FunctionOptimizationType::kSimple, VerificationType::kIdentity, test_matrix_prod),
        };
    TestRunner test_runner(&tasks);
    test_runner.run_all(std::cout);
    return 0;
}
