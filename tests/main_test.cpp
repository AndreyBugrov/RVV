#include <iostream>
#include <map>

#include "test_runner.hpp"  // TestRunner class
#include "test_set.hpp"  // list of all tests

using std::pair;
using std::string;
using std::map;
using std::function;

enum object_indexes{
    kScalarMultiplication,
    kVectorNormCalculation,
    kVectorNormalization,
    kMatrixProduct,
    kMatrixTransposition,
    kGramSchmidtProcess,
    kQRDecompostion,
};

void fill_tasks(std::vector<TestTask>& tasks){
    const string object_names[] ={
        "vectors)",
        "vector)",
        "vector)",
        "matrices)",
        "matrix)",
        "vector system)",
        "matrix)"
    };

    const map<object_indexes, map<FunctionOptimizationType, string>> function_types={
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kScalarMultiplication,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimple, "simple scalar product"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimpleStd, "STD-based scalar product"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnsafe, "STD-based unsafe scalar product"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kVectorNormCalculation,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kNoThrowing, "STD-based vector norm calculaion"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kVectorNormalization,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kNoThrowing, "inplace vector normalization"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kMatrixProduct,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimple, "base simple matrix product"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kMatrixTransposition,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kNoThrowing, "base simple matrix transposition"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kGramSchmidtProcess,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnsafe, "base unsafe Gram-Schmidt process"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kQRDecompostion,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnsafe, "base unsafe QR decomposition"),
            }
        ),
    };
    const map<AlgebraObjectVersion, string> verification_names={
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kEmpty, " (empty "),
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kZero, " (zero "),
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kIdentity, " (identity "),
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kGeneral, " (general "),
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kWrong, " (incorrect "),
    };
    const map<object_indexes, function<AssertionResult(TestFunctionInputExtended)>> object_types={
        pair<object_indexes, function<AssertionResult(TestFunctionInputExtended)>>(object_indexes::kScalarMultiplication, test_scalar_prod),
        pair<object_indexes, function<AssertionResult(TestFunctionInputExtended)>>(object_indexes::kVectorNormCalculation, test_vector_norm),
        pair<object_indexes, function<AssertionResult(TestFunctionInputExtended)>>(object_indexes::kVectorNormalization, test_normalize_vector),
        pair<object_indexes, function<AssertionResult(TestFunctionInputExtended)>>(object_indexes::kMatrixProduct, test_matrix_prod),
        pair<object_indexes, function<AssertionResult(TestFunctionInputExtended)>>(object_indexes::kMatrixTransposition, test_matrix_transposition),
        pair<object_indexes, function<AssertionResult(TestFunctionInputExtended)>>(object_indexes::kGramSchmidtProcess, test_qram_schmidt),
        pair<object_indexes, function<AssertionResult(TestFunctionInputExtended)>>(object_indexes::kQRDecompostion, test_qr_decomposition),
    };

    for(const pair<object_indexes, function<AssertionResult(TestFunctionInputExtended)>> object_type_pair : object_types){
        for(const pair<FunctionOptimizationType, string> function_type : function_types.at(object_type_pair.first)){
            for(const pair<AlgebraObjectVersion, string> verification_name_pair: verification_names){
                if(!((verification_name_pair.first==AlgebraObjectVersion::kWrong)&&(function_type.first==FunctionOptimizationType::kUnsafe || function_type.first == FunctionOptimizationType::kNoThrowing))){
                    tasks.push_back(TestTask(function_type.second+verification_name_pair.second+object_names[object_type_pair.first], function_type.first, verification_name_pair.first, object_type_pair.second));
                }
            }
        }
    }
}

int main(){

    std::vector<TestTask> tasks;
    fill_tasks(tasks);
    TestRunner test_runner(&tasks);
    test_runner.run_all(std::cout);
    return 0;
}
