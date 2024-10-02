#include <iostream>

#include "test_runner.hpp"

int main(){
    BaseTaskRunner* test_runner = new TestRunner;
    test_runner->run_all();
    return 0;
}
