#include <iostream>

#include "test_runner.hpp"  // TestRunner class
#include "test_set.hpp"  // list of all tests

using namespace assert;

int main(){
    std::vector<TestTask> tasks{
        TestTask("always failing", test_always_failing),
        TestTask("assert any exception", test_assert_any_throw),
        TestTask("assert no exception", test_assert_no_throw),
        TestTask("assert specified exception", test_assert_throw),
        TestTask("scalar product empty vectors", test_scalar_product_simple_empty_vectors),
        TestTask("inner product empty vectors", test_scalar_product_std_empty_vectors),
        TestTask("scalar product zero vectors", test_scalar_product_simple_zero_vectors), 
        TestTask("inner product zero vectors", test_scalar_product_std_zero_vectors),
        TestTask("scalar product one", test_scalar_product_simple_one),
        TestTask("inner product one", test_scalar_product_std_one),
        TestTask("scalar product universal", test_scalar_product_universal),
        TestTask("throws when lengths of vectors are unequal", test_scalar_product_simple_different_length_of_vectors),
        };
    TestRunner test_runner(&tasks);
    test_runner.run_all(std::cout);
    return 0;
}
