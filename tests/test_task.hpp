#pragma once

#include "../tasks/base_task.hpp"  // inheritance
#include "../common/exception.hpp"  // Exception
#include "expect.hpp"  // ExpectationResult

const num_type kMinValue = -100.0;
const num_type kMaxValue = 100.0;

struct TestFunctionInput{
public:
    num_type min_value;
    num_type max_value;
    size_t min_length;
    size_t max_length;
    TestFunctionInput(double input_min_value=0.0, double input_max_value=0.0, size_t input_min_length=0, size_t input_max_length=0):
    min_value(input_min_value), max_value(input_max_value), min_length(input_min_length), max_length(input_max_length)
    {}
    TestFunctionInput(const TestFunctionInput& input): min_value(input.min_value), max_value(input.max_value),
    min_length(input.min_length), max_length(input.max_length){}
};

enum class FunctionOptimizationType{
    kSimple,
    kStd,
    kRow,
    kRowRow,
    kRowScalar,
    kSimd,
    kUnrolling,
    kDoubleUnrolling,
    kBlock,
    kBlockScalar,
    kBlockScalarPar,
    kInline,
    kMatrix,
    kUnsafe,
    kHouseholder,
    kInlinePar,
    kNoThrowing,
};

enum class AlgebraObjectVersion{
    kEmpty = -1,
    kZero,
    kIdentity, // one object is identity
    kGeneral,
    kIncorrect, // check throws or not in wrong input case
};

struct TestFunctionInputExtended: TestFunctionInput{
public:
    FunctionOptimizationType function_type;
    AlgebraObjectVersion algebra_object_version;
    TestFunctionInputExtended(const TestFunctionInput& base, FunctionOptimizationType input_function_type, AlgebraObjectVersion input_version):
    TestFunctionInput(base), function_type(input_function_type), algebra_object_version(input_version)
    {}
};


class TestOutput: public BaseTaskOutput{
protected:
    bool passed_;
    
public:
    TestOutput(bool ended, std::string error_type, std::string error_msg, double seconds, bool passed): BaseTaskOutput(ended, error_type, error_msg, seconds), passed_(passed){}
    bool passed() const{return passed_;}
};

class TestTask: public BaseTask<TestFunctionInputExtended, ExpectationResult, TestOutput>{
    FunctionOptimizationType function_type_;
    AlgebraObjectVersion version_;
public:
    TestTask(std::string name, FunctionOptimizationType function_type, AlgebraObjectVersion version, 
    std::function<ExpectationResult(TestFunctionInputExtended)> task): 
    BaseTask(name, task), function_type_(function_type), version_(version)
    {}
    TestOutput run(TestFunctionInput input) const{
        bool ended = false;
        std::string error_type;
        std::string error_message;
        double seconds = 0.0;
        ExpectationResult passed = false;
        try{
            const auto start_test{std::chrono::steady_clock::now()};
            passed = task_(TestFunctionInputExtended(input, function_type_, version_));
            const auto end_test{std::chrono::steady_clock::now()};
            ended = true;
            std::chrono::duration<double> test_seconds = end_test - start_test;
            if(!passed){
                error_type = "";
                error_message = passed.error_message();
            }
            seconds = test_seconds.count();
        }
        catch(const Exception& my_error){
            error_type = my_error.what();
            error_message = my_error.message();
        }
        catch(const std::exception& ex){
            error_type = "std::exception";
            error_message = ex.what();
        }
        catch(...){
            error_type = "Unknown";
            error_message = "Unknown";
        }
        return TestOutput(ended, error_type, error_message, seconds, passed);
    }
};
