#include <iostream>

#include "test_runner.hpp"  // TestRunner class
#include "test_set.hpp"  // list of all tests

using namespace assert;

int main(){
    std::vector<TestTask> tasks{
        TestTask("test assert any exception", test_assert_any_throw),
        TestTask("test assert no exception", test_assert_no_throw),
        TestTask("test assert specified exception", test_assert_throw),
        TestTask("test scalar product empty vectors", test_scalar_product_simple_empty_vectors),
        TestTask("test inner product empty vectors", test_scalar_product_std_empty_vectors),
        TestTask("test scalar product zero vectors", test_scalar_product_simple_zero_vectors), 
        TestTask("test inner product zero vectors", test_scalar_product_std_zero_vectors),
        TestTask("test scalar product one", test_scalar_product_simple_one),
        TestTask("test inner product one", test_scalar_product_std_one),
        TestTask("test scalar product universal", test_scalar_product_universal),
        };
    TestRunner test_runner(&tasks);
    test_runner.run_all(std::cout);
    return 0;
}
