#include <iostream>

#include "test_runner.hpp"

int main(){
    std::vector<TestTask> tasks{TestTask(std::string("my mult"), std::function<bool(TestFunctionInput)>(test_scalar_product_std_empty_vectors)), TestTask("not my mult", test_scalar_product_simple_empty_vectors)};
    TestRunner test_runner(&tasks);
    test_runner.run_all(std::cout);
    return 0;
}
