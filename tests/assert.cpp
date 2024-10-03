#include "assert.hpp"

bool assert::assert_true(bool expression){
    if(expression){
        return true;
    }
    std::ostringstream error_message;
    error_message<<"Expected: true statement. Actual: "<<expression;
    throw AssertionError(error_message.str()); 
}

bool assert::assert_false(bool expression){
    if(!expression){
        return true;
    }
    std::ostringstream error_message;
    error_message<<"Expected: false statement. Actual: "<<expression;
    throw AssertionError(error_message.str()); 
}

