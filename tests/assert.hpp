#pragma once

#include <sstream>  // stringstream
#include <cmath>  // fabs

#include "../common/exception.hpp" // AssertionError

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
    template <typename F, typename ExcType, typename... Args>
    AssertionResult assert_throw(F foo, ExcType exc, Args... args){
        try{
            foo(args...);
        }catch(ExcType ex){
            return AssertionResult(true);
        }catch(Exception ex){
            std::ostringstream error_message;
            error_message<<"Expected: "<<exc.what()<<". Actual: "<<ex.what();
            return AssertionResult(false, error_message.str());
        }
        catch(std::exception ex){
            std::ostringstream error_message;
            error_message<<"Expected: "<<exc.what()<<". Actual: "<<ex.what();
            return AssertionResult(false, error_message.str());
        }catch(...){
            std::ostringstream error_message;
            error_message<<"Expected: "<<exc.what()<<". Actual: unknown exception";
            return AssertionResult(false, error_message.str());
        }
        std::ostringstream error_message;
        error_message<<"Expected: "<<exc.what()<<". Actual: no exception";
        return AssertionResult(false, error_message.str());
    }

    template <typename Foo, typename... Args>
    AssertionResult assert_no_throw(Foo foo, Args... args){
        try{
            foo(args...);
        }catch(...){
            std::ostringstream error_message;
            error_message<<"Expected: function does not throw exception. Actual: it does";
            return AssertionResult(false, error_message.str());
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
        std::ostringstream error_message;
        error_message<<"Expected: function throws exception. Actual: it does not";
        return AssertionResult(false, error_message.str());
    }

    template<typename T>
    AssertionResult assert_eq(T expected, T actual){
        if(expected == actual){
            return AssertionResult(true);
        }
        std::ostringstream error_message;
        error_message<<"Expected: equality to "<<expected<<". Actual: "<<actual;
        return AssertionResult(false, error_message.str());
    }

    template<typename T>
    AssertionResult assert_neq(T expected, T actual){
        if(expected != actual){
            return AssertionResult(true);
        }
        std::ostringstream error_message;
        error_message<<"Expected: inequality to "<<expected<<". Actual: "<<actual;
        return AssertionResult(false, error_message.str());
    }

    template<typename T>
    AssertionResult assert_near(T expected, T actual, T abs_error){
        if(std::fabs(expected-actual) <= abs_error){
            return AssertionResult(true);
        }
        std::ostringstream error_message;
        error_message<<"Expected: equality to "<<expected<<" near "<<abs_error<<". Actual: "<<actual;
        return AssertionResult(false, error_message.str());
    }

    AssertionResult assert_true(bool expression);
    AssertionResult assert_false(bool expression);
}

