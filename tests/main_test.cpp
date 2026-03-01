#include <iostream>
#include <map>

#include "test_runner.hpp"  // TestRunner class
#include "test_set.hpp"  // list of all tests

using std::pair;
using std::string;
using std::map;
using std::function;

enum object_indexes{
    kDotProduct,
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
            object_indexes::kDotProduct,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimple, "simple dot product"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kStd, "STD-based dot product"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimd, "dot product with omp simd"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnrolling, "dot product with loop unrolling"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnsafe, "unsafe dot product"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kVectorNormCalculation,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kNoThrowing, "vector norm calculation"),
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
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kRow, "row simple matrix product"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kBlock, "block row matrix product"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kBlockScalar, "block row matrix product with unrolling"),
                
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kMatrixTransposition,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kNoThrowing, "matrix transposition"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kGramSchmidtProcess,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimple, "base Gram-Schmidt process"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kRow, "matrix-based Gram-Schmidt process"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimd, "matrix-based Gram-Schmidt process"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnrolling, "Gram-Schmidt process with unrolling"),
            }
        ),
        pair<object_indexes, map<FunctionOptimizationType, string>>(
            object_indexes::kQRDecompostion,
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimple, "base QR decomposition"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kRow, "row product QR decomposition"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kRowRow, "row product + row process QR decomposition"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimd, "QR decomposition with omp simd"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnrolling, "QR decomposition with half-unrolling"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kDoubleUnrolling, "QR decomposition with full unrolling"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kHouseholder, "QR decomposition Householder"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kBlock, "QR decomposition with block matrix product"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kBlockScalar, "QR decomposition with optimal block matrix product"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kInline, "QR decomposition with inlining"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kMatrix, "QR decomposition full matrix"),
                // pair<FunctionOptimizationType, string>(FunctionOptimizationType::kInlinePar, "Parallel QR decomposition with inlining"),
            }
        ),
    };

    const map<AlgebraObjectVersion, string> verification_names={
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kEmpty, " (empty "),
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kZero, " (zero "),
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kIdentity, " (identity "),
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kGeneral, " (general "),
        pair<AlgebraObjectVersion, string>(AlgebraObjectVersion::kIncorrect, " (incorrect "),
    };

    const map<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>> object_types={
        pair<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>>(object_indexes::kDotProduct, test_dot_product),
        pair<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>>(object_indexes::kVectorNormCalculation, test_vector_norm),
        pair<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>>(object_indexes::kVectorNormalization, test_normalize_vector),
        pair<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>>(object_indexes::kMatrixProduct, test_matrix_product),
        pair<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>>(object_indexes::kMatrixTransposition, test_matrix_transposition),
        pair<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>>(object_indexes::kGramSchmidtProcess, test_gram_schmidt),
        pair<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>>(object_indexes::kQRDecompostion, test_qr_decomposition),
    };

    for(const pair<object_indexes, function<ExpectationResult(TestFunctionInputExtended)>> object_type_pair : object_types){
        for(const pair<FunctionOptimizationType, string> function_type : function_types.at(object_type_pair.first)){
            for(const pair<AlgebraObjectVersion, string> verification_name_pair: verification_names){
                if(!((verification_name_pair.first==AlgebraObjectVersion::kIncorrect)&&(function_type.first==FunctionOptimizationType::kUnsafe || function_type.first == FunctionOptimizationType::kNoThrowing))){
                    tasks.push_back(TestTask(function_type.second+verification_name_pair.second+object_names[object_type_pair.first], function_type.first, verification_name_pair.first, object_type_pair.second));
                }
            }
        }
    }
}

int main(){
    std::vector<TestTask> tasks;
    fill_tasks(tasks);
    int failed_test_count = static_cast<int>(run_tests(tasks, tasks.size(), std::cout));
    return failed_test_count;
}
