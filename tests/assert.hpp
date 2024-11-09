#pragma once

#include <sstream>  // stringstream
#include <cmath>  // fabs
#include <limits> // max double value

#include "../common/exception.hpp" // AssertionError
#include "../common/generators.hpp"  // generate_string

#include <iostream>

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

    // WARNING: Any function is not noexcept if you pass non-writable type to it

    template <typename F, typename... Args>
    AssertionResult assert_throw(F foo, Exception exc, Args... args) noexcept {
        try{
            foo(args...);
        }catch(const Exception& ex){
            if (ex.what() == exc.what()){
                return AssertionResult(true);
            }
            else{
                return AssertionResult(false, generate_string("Expected: ", exc.what(), ". Actual: ", ex.what()));
            }
        }
        catch(std::exception* ex){
            return AssertionResult(false, generate_string("Expected: ", exc.what(), ". Actual: ", ex->what()));
        }catch(...){
            return AssertionResult(false, generate_string("Expected: ", exc.what(), ". Actual: unknown exception"));
        }
        return AssertionResult(false, generate_string("Expected: ", exc.what(), ". Actual: no exception"));
    }

    template <typename Foo, typename... Args>
    AssertionResult assert_no_throw(Foo foo, Args... args) noexcept {
        try{
            foo(args...);
        }catch(...){
            return AssertionResult(false, "Expected: function does not throw exception. Actual: it does");
        }
        return AssertionResult(true);
    }

    template <typename Foo, typename... Args>
    AssertionResult assert_any_throw(Foo foo, Args... args) noexcept {
        try{
            foo(args...);
        }catch(...){
            return AssertionResult(true);
        }
        return AssertionResult(false, "Expected: function throws exception. Actual: it does not");
    }

    template<typename T>
    AssertionResult assert_eq(T expected, T actual) noexcept {
        if(expected == actual){
            return AssertionResult(true);
        }
        return AssertionResult(false, generate_string("Expected: equality to ", expected, ". Actual: ", actual));
    }

    template<typename Container>
    AssertionResult assert_iterable_containers_eq(Container expected, Container actual, size_t length) noexcept {
        num_type max_difference = 0.0;
        num_type min_difference = std::numeric_limits<num_type>::max();
        size_t wrong_value_number = 0;
        for(size_t i = 0; i<length;++i){
            num_type difference = std::fabs(expected[i]-actual[i]);
            if(difference){
                // std::cout<<difference<<"\n";
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
            return AssertionResult(false, generate_string("Wrong value percentage: ", wrong_value_percentage, "%. Min difference: ", min_difference ,". Max difference: ", max_difference, ". Container length: ", length));
        }else{
            return AssertionResult(true);
        }
    }

    template<typename Container, typename T>
    AssertionResult assert_iterable_containers_near(Container expected, Container actual, T eps, size_t length) noexcept {
        T max_difference = 0.0;
        T min_difference = std::numeric_limits<num_type>::max();
        size_t wrong_value_number = 0;
        for(size_t i = 0; i<length;++i){
            T difference = std::fabs(expected[i]-actual[i]);
            if(difference > eps){
                // std::cout<<difference<<"\n";
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
            return AssertionResult(false, generate_string("Wrong value percentage: ", wrong_value_percentage, "%. Min difference: ", min_difference ,". Max difference: ", max_difference, ". Container length: ", length));
        }else{
            return AssertionResult(true);
        }
    }

    template<typename T>
    AssertionResult assert_neq(T expected, T actual) noexcept {
        if(expected != actual){
            return AssertionResult(true);
        }
        return AssertionResult(false, generate_string("Expected: inequality to ", expected, ". Actual: ", actual));
    }

    template<typename T>
    AssertionResult assert_near(T expected, T actual, T abs_error) noexcept {
        try{
            T result = std::fabs(expected-actual);
            if(result <= abs_error){
                return AssertionResult(true);
            }
            return AssertionResult(false, generate_string("Expected: equality to ", expected, " near ", abs_error, ". Actual: ", actual));
        }
        catch(...){
            return AssertionResult(false, generate_string("Can't calculate modulus of difference between ", expected, " and ", actual));
        }
    }

    AssertionResult assert_true(bool expression) noexcept;
    AssertionResult assert_false(bool expression) noexcept;
}

