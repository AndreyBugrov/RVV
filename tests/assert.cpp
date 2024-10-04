#include "assert.hpp"

AssertionResult assert::assert_true(bool expression){
    if(expression){
        return AssertionResult(true);
    }
    std::ostringstream error_message;
    error_message<<"Expected: true statement. Actual: "<<expression;
    return AssertionResult(false, error_message.str());
}

AssertionResult assert::assert_false(bool expression){
    if(!expression){
        return AssertionResult(true);
    }
    std::ostringstream error_message;
    error_message<<"Expected: false statement. Actual: "<<expression;
    return AssertionResult(false, error_message.str());
}

