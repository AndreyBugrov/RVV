#include "expect.hpp"

ExpectationResult expect::expect_true(bool expression) noexcept {
    if(expression){
        return ExpectationResult(true);
    }
    return ExpectationResult(false, "Expected: true statement. Actual: false");
}

ExpectationResult expect::expect_false(bool expression) noexcept {
    if(!expression){
        return ExpectationResult(true);
    }
    return ExpectationResult(false, "Expected: false statement. Actual: true");
}

