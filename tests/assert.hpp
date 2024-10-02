#pragma once

#include <sstream>  // stringstream
#include <cmath>  // fabs

#include "../common/exception.hpp" // AssertionError

template <class T>
class Assert{
public:
    static bool assert_eq(T expected, T actual){
        if(expected == actual){
            return true;
        }
        std::ostringstream error_message;
        error_message<<"Expected: equality to "<<expected<<". Actual: "<<actual;
        throw AssertionError(error_message.str());
    }
    static bool assert_neq(T expected, T actual){
        if(expected != actual){
            return true;
        }
        std::ostringstream error_message;
        error_message<<"Expected: inequality to "<<expected<<". Actual: "<<actual;
        throw AssertionError(error_message.str());
    }
    static bool assert_near(T expected, T actual, T abs_error){
        if(std::fabs(expected-actual) <= abs_error){
            return true;
        }
        std::ostringstream error_message;
        error_message<<"Expected: equality to "<<expected<<" near "<<abs_error<<". Actual: "<<actual;
        throw AssertionError(error_message.str());
    }
};