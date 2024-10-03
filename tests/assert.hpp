#pragma once

#include <sstream>  // stringstream
#include <cmath>  // fabs

#include "../common/exception.hpp" // AssertionError

namespace assert{
    template <typename F, typename ExcType, typename... Args>
    bool assert_exception(F foo, ExcType exc, Args... args){
        try{
            foo(args...);
        }catch(ExcType ex){
            return true;
        }catch(std::exception ex){
            std::ostringstream error_message;
            error_message<<"Expected: exception "<<exc.what()<<". Actual: "<<ex.what();
            throw AssertionError(error_message.str());
        }catch(...){
            std::ostringstream error_message;
            error_message<<"Expected: exception "<<exc.what()<<". Actual: unknown exception";
            throw AssertionError(error_message.str());
        }
        std::ostringstream error_message;
        error_message<<"Expected: exception "<<exc.what()<<". Actual: no exception";
        throw AssertionError(error_message.str());
    }

    template <typename Foo, typename... Args>
    bool assert_any_exception(Foo foo, Args... args){
        try{
            foo(args...);
        }catch(...){
            return true;
        }
        std::ostringstream error_message;
        error_message<<"Expected: exception. Actual: no exception";
        throw AssertionError(error_message.str());
    }

    template<typename T>
    bool assert_eq(T expected, T actual){
        if(expected == actual){
            return true;
        }
        std::ostringstream error_message;
        error_message<<"Expected: equality to "<<expected<<". Actual: "<<actual;
        throw AssertionError(error_message.str());
    }

    template<typename T>
    bool assert_neq(T expected, T actual){
        if(expected != actual){
            return true;
        }
        std::ostringstream error_message;
        error_message<<"Expected: inequality to "<<expected<<". Actual: "<<actual;
        throw AssertionError(error_message.str());
    }

    template<typename T>
    bool assert_near(T expected, T actual, T abs_error){
        if(std::fabs(expected-actual) <= abs_error){
            return true;
        }
        std::ostringstream error_message;
        error_message<<"Expected: equality to "<<expected<<" near "<<abs_error<<". Actual: "<<actual;
        throw AssertionError(error_message.str());
    }

    bool assert_true(bool expression);
    bool assert_false(bool expression);
}

