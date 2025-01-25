#pragma once

#include <cmath>  // fabs
#include <limits> // max double value

#include "../common/exception.hpp" // Exception in expect_throw
#include "../common/generators.hpp"  // generate_string

class ExpectationResult{
protected:
    bool passed_;
    std::string error_message_;
public:
    ExpectationResult(bool passed, std::string error_message=""): passed_(passed), error_message_(error_message){}
    ~ExpectationResult() = default;
    operator bool()const {return passed_;}
    std::string error_message() const {return error_message_;}
};

namespace expect{

    // WARNING: Any function is not noexcept if you pass non-writable type to it

    template <typename F, typename... Args>
    ExpectationResult expect_throw(F foo, Exception exc, Args... args) noexcept {
        try{
            foo(args...);
        }catch(const Exception& ex){
            if (ex.what() == exc.what()){
                return ExpectationResult(true);
            }
            else{
                return ExpectationResult(false, generate_string("Expected: ", exc.what(), ". Actual: ", ex.what()));
            }
        }
        catch(std::exception* ex){
            return ExpectationResult(false, generate_string("Expected: ", exc.what(), ". Actual: ", ex->what()));
        }catch(...){
            return ExpectationResult(false, generate_string("Expected: ", exc.what(), ". Actual: unknown exception"));
        }
        return ExpectationResult(false, generate_string("Expected: ", exc.what(), ". Actual: no exception"));
    }

    template <typename Foo, typename... Args>
    ExpectationResult expect_no_throw(Foo foo, Args... args) noexcept {
        try{
            foo(args...);
        }catch(...){
            return ExpectationResult(false, "Expected: function does not throw exception. Actual: it does");
        }
        return ExpectationResult(true);
    }

    template <typename Foo, typename... Args>
    ExpectationResult expect_any_throw(Foo foo, Args... args) noexcept {
        try{
            foo(args...);
        }catch(...){
            return ExpectationResult(true);
        }
        return ExpectationResult(false, "Expected: function throws exception. Actual: it does not");
    }

    template<typename T>
    ExpectationResult expect_eq(T expected, T actual) noexcept {
        if(expected == actual){
            return ExpectationResult(true);
        }
        return ExpectationResult(false, generate_string("Expected: equality to ", expected, ". Actual: ", actual));
    }

    template<typename Container>
    ExpectationResult expect_iterable_containers_eq(Container expected, Container actual, size_t length) noexcept {
        num_type max_difference = 0.0;
        num_type min_difference = std::numeric_limits<num_type>::max();
        size_t wrong_value_number = 0;
        for(size_t i = 0; i<length;++i){
            num_type difference = std::fabs(expected[i]-actual[i]);
            if(difference){
                ++wrong_value_number;
                if(difference>max_difference){
                    max_difference = difference;
                }
                if(difference<min_difference){
                    min_difference = difference;
                }
            }
        }
        if(wrong_value_number){
            double wrong_value_percentage = double(wrong_value_number) / double(length) * 100.0;
            return ExpectationResult(false, generate_string("Wrong value percentage: ", wrong_value_percentage, "%. Min difference: ", min_difference ,". Max difference: ", max_difference, ". Container length: ", length));
        }else{
            return ExpectationResult(true);
        }
    }

    template<typename Container, typename T>
    ExpectationResult expect_iterable_containers_near(Container expected, Container actual, T eps, size_t length) noexcept {
        T max_difference = 0.0;
        T min_difference = std::numeric_limits<num_type>::max();
        size_t wrong_value_number = 0;
        for(size_t i = 0; i<length;++i){
            T difference = std::fabs(expected[i]-actual[i]);
            if(difference > eps){
                ++wrong_value_number;
                if(difference>max_difference){
                    max_difference = difference;
                }
                if(difference<min_difference){
                    min_difference = difference;
                }
            }
        }
        if(wrong_value_number){
            double wrong_value_percentage = double(wrong_value_number) / double(length) * 100.0;
            return ExpectationResult(false, generate_string("Wrong value percentage: ", wrong_value_percentage, "%. Min difference: ", min_difference ,". Max difference: ", max_difference, ". Container length: ", length));
        }else{
            return ExpectationResult(true);
        }
    }

    template<typename T>
    ExpectationResult expect_neq(T expected, T actual) noexcept {
        if(expected != actual){
            return ExpectationResult(true);
        }
        return ExpectationResult(false, generate_string("Expected: inequality to ", expected, ". Actual: ", actual));
    }

    template<typename T>
    ExpectationResult expect_near(T expected, T actual, T abs_error) noexcept {
        try{
            T result = std::fabs(expected-actual);
            if(result <= abs_error){
                return ExpectationResult(true);
            }
            return ExpectationResult(false, generate_string("Expected: equality to ", expected, " near ", abs_error, ". Actual: ", actual));
        }
        catch(...){
            return ExpectationResult(false, generate_string("Can't calculate modulus of difference between ", expected, " and ", actual));
        }
    }

    ExpectationResult expect_true(bool expression) noexcept;
    ExpectationResult expect_false(bool expression) noexcept;
}

