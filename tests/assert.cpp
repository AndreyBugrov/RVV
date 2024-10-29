#include "assert.hpp"

AssertionResult assert::assert_true(bool expression) noexcept {
    if(expression){
        return AssertionResult(true);
    }
    return AssertionResult(false, "Expected: true statement. Actual: false");
}

AssertionResult assert::assert_false(bool expression) noexcept {
    if(!expression){
        return AssertionResult(true);
    }
    return AssertionResult(false, "Expected: false statement. Actual: true");
}

