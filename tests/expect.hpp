#pragma once

#include <cmath>  // fabs
#include <limits> // max double value
#include <concepts> // concept

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

template<typename T>
concept is_not_void_type = !std::is_void_v<T>;

template <class T>
concept Indexable = requires (T series){
    {series.begin()} -> std::random_access_iterator;
}
&& requires (T series){
    {series.end()} -> std::random_access_iterator;
} || requires(T series, size_t index){
    {series[index]} -> is_not_void_type;
};

template <class T>
concept FabsSupporting = requires (T value){
    {fabs(value)} -> std::same_as<T>;
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

    template<typename Container, typename T>
    requires Indexable<Container> && FabsSupporting<T>
    ExpectationResult expect_indexable_containers_near(Container expected, Container actual, T eps, size_t length) noexcept {
        T max_difference = 0.0;
        T min_difference = std::numeric_limits<num_type>::max();
        size_t wrong_value_number = 0;
        size_t first_wrong_index = 0;
        size_t last_wrong_index = 0;
        for(size_t i = 0; i<length;++i){
            T difference = std::fabs(expected[i] - actual[i]);
            if(difference > eps){
                if(!wrong_value_number){
                    first_wrong_index = i;
                }else{
                    last_wrong_index = i;
                }
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
            double wrong_value_percentage = double(wrong_value_number) / double(length) * 100.0; // only double, not num_type
            return ExpectationResult(false, generate_string("Wrong value percentage: ", wrong_value_percentage, "%. Absolute min difference: ", min_difference ,". Absolute max difference: ", max_difference, ". Container length: ", length, ". First wrong index: ", first_wrong_index, ". Last wrong index: ", last_wrong_index));
        }else{
            return ExpectationResult(true);
        }
    }

    template<typename Container, typename T=num_type>
    requires Indexable<Container> && FabsSupporting<T>
    ExpectationResult expect_indexable_containers_eq(Container expected, Container actual, size_t length) noexcept {
        return expect_indexable_containers_near(expected, actual, T(0.0), length);
    }

    template<typename Container, typename T>
    requires Indexable<Container> && FabsSupporting<T>
    ExpectationResult expect_indexable_containers_near_relative(Container expected, Container actual, T relative_eps, size_t length) noexcept {
        T max_difference = 0.0;
        T min_difference = std::numeric_limits<num_type>::max();
        size_t wrong_value_number = 0;
        size_t first_wrong_index = 0;
        size_t last_wrong_index = 0;
        T zero = 0.0;
        T difference = 0.0;
        for(size_t i = 0; i<length;++i){
            if(expected[i] == zero){
                difference = std::fabs(expected[i] - actual[i]);
            }else{
                difference = std::fabs(expected[i] - actual[i]) / std::fabs(expected[i]);
            }
            if(difference > relative_eps){
                if(!wrong_value_number){
                    first_wrong_index = i;
                }else{
                    last_wrong_index = i;
                }
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
            double wrong_value_percentage = double(wrong_value_number) / double(length) * 100.0; // only double, not num_type
            return ExpectationResult(false, generate_string("Wrong value percentage: ", wrong_value_percentage, "%. Relative min difference: ", min_difference ,". Relative max difference: ", max_difference, ". Container length: ", length, ". First wrong index: ", first_wrong_index, ". Last wrong index: ", last_wrong_index));
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
    requires FabsSupporting<T>
    ExpectationResult expect_near(T expected, T actual, T abs_error) noexcept {
        T result = std::fabs(expected-actual);
        if(result <= abs_error){
            return ExpectationResult(true);
        }
        return ExpectationResult(false, generate_string("Expected: equality to ", expected, " near ", abs_error, ". Actual: ", actual));
    }

    ExpectationResult expect_true(bool expression) noexcept;
    ExpectationResult expect_false(bool expression) noexcept;
}

