#include <iostream>

#include "test_runner.hpp"

int main(){
    TestRunner test_runner;
    test_runner.run_all(std::cout);
    return 0;
}
