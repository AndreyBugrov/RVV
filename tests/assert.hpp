#pragma once

#include <sstream>  // stringstream
#include <cmath>  // fabs

#include "../common/exception.hpp" // AssertionError
#include "../common/generators.hpp"  // generate_string

class AssertionResult{
protected:
    bool passed_;
    std::string error_message_;
public:
    AssertionResult(bool passed, std::string error_message=""): passed_(passed), error_message_(error_message){}
    ~AssertionResult() = default;
    operator bool()const {return passed_;}
    std::string error_message() const {return error_message_;}
};

namespace assert{
    template <typename F, typename... Args>
    AssertionResult assert_throw(F foo, Exception exc, Args... args){
        try{
            foo(args...);
        }catch(Exception ex){
            if (ex.what() == exc.what()){
                return AssertionResult(true);
            }
            else{
                return AssertionResult(false, generate_string("Expected: ", exc.what(), ". Actual: ", ex.what()));
            }
        }
        catch(std::exception ex){
            return AssertionResult(false, generate_string("Expected: ", exc.what(), ". Actual: ", ex.what()));
        }catch(...){
            return AssertionResult(false, generate_string("Expected: ", exc.what(), ". Actual: unknown exception"));
        }
        return AssertionResult(false, generate_string("Expected: ", exc.what(), ". Actual: no exception"));
    }

    template <typename Foo, typename... Args>
    AssertionResult assert_no_throw(Foo foo, Args... args){
        try{
            foo(args...);
        }catch(...){
            return AssertionResult(false, "Expected: function does not throw exception. Actual: it does");
        }
        return AssertionResult(true);
    }

    template <typename Foo, typename... Args>
    AssertionResult assert_any_throw(Foo foo, Args... args){
        try{
            foo(args...);
        }catch(...){
            return AssertionResult(true);
        }
        return AssertionResult(false, "Expected: function throws exception. Actual: it does not");
    }

    template<typename T>
    AssertionResult assert_eq(T expected, T actual){
        if(expected == actual){
            return AssertionResult(true);
        }
        return AssertionResult(false, generate_string("Expected: equality to ", expected, ". Actual: ", actual));
    }

    template<typename T>
    AssertionResult assert_neq(T expected, T actual){
        if(expected != actual){
            return AssertionResult(true);
        }
        return AssertionResult(false, generate_string("Expected: inequality to ", expected, ". Actual: ", actual));
    }

    template<typename T>
    AssertionResult assert_near(T expected, T actual, T abs_error){
        if(std::fabs(expected-actual) <= abs_error){
            return AssertionResult(true);
        }
        return AssertionResult(false, generate_string("Expected: equality to ", expected, " near ", abs_error, ". Actual: ", actual));
    }

    AssertionResult assert_true(bool expression);
    AssertionResult assert_false(bool expression);
}

