#include <iostream>
#include <map>

#include "test_runner.hpp"  // TestRunner class
#include "test_set.hpp"  // list of all tests

using std::pair;
using std::string;
using std::map;
using std::function;

enum object_indexes{
    kVectorMultiplication,
    kVectorNormCalculation,
    kMatrixMultiplication,
    kGramSchmidtProcess,
    kQRDecompostion,
};

void fill_tasks(std::vector<TestTask>& tasks){
    const string object_names[] ={
        "vectors)",
        "vector)",
        "matrices)",
        "vector system)",
        "matrix)"
    };

    const map<string, map<FunctionOptimizationType, string>> function_types={
        pair<string, map<FunctionOptimizationType, string>>(
            object_names[kVectorMultiplication],
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimple, "simple scalar product"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimpleStd, "STD-based scalar product"),
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnsafe, "STD-based unsafe scalar product"),
            }
        ),
        pair<string, map<FunctionOptimizationType, string>>(
            object_names[kVectorNormCalculation],
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnsafe, "STD-based unsafe vector norm calulaion"),
            }
        ),
        pair<string, map<FunctionOptimizationType, string>>(
            object_names[kMatrixMultiplication],
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kSimple, "base simple matrix product"),
            }
        ),
        pair<string, map<FunctionOptimizationType, string>>(
            object_names[kGramSchmidtProcess],
            map<FunctionOptimizationType, string>{
                pair<FunctionOptimizationType, string>(FunctionOptimizationType::kUnsafe, "base unsafe Gram-Schmidt process"),
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
    const map<string, function<AssertionResult(TestFunctionInputExtended)>> object_types={
        pair<string, function<AssertionResult(TestFunctionInputExtended)>>(object_names[kVectorMultiplication], test_scalar_prod),
        pair<string, function<AssertionResult(TestFunctionInputExtended)>>(object_names[kVectorNormCalculation], test_vector_norm),
        pair<string, function<AssertionResult(TestFunctionInputExtended)>>(object_names[kMatrixMultiplication], test_matrix_prod),
        pair<string, function<AssertionResult(TestFunctionInputExtended)>>(object_names[kGramSchmidtProcess], test_qram_schmidt),
    };

    for(const pair<string, function<AssertionResult(TestFunctionInputExtended)>> object_type_pair : object_types){
        for(const pair<FunctionOptimizationType, string> function_type : function_types.at(object_type_pair.first)){
            for(const pair<AlgebraObjectVersion, string> verification_name_pair: verification_names){
                if(!((verification_name_pair.first==AlgebraObjectVersion::kWrong)&&(function_type.first==FunctionOptimizationType::kUnsafe))){
                    tasks.push_back(TestTask(function_type.second+verification_name_pair.second+object_type_pair.first, function_type.first, verification_name_pair.first, object_type_pair.second));
                }
            }
        }
    }
}

int main(){

    std::vector<TestTask> tasks{
        // TestTask("always failing", test_always_failing),
        // TestTask("scalar product empty vectors", test_scalar_product_simple_empty_vectors),
        // TestTask("inner product empty vectors", test_scalar_product_std_empty_vectors),
        // TestTask("scalar product zero vectors", test_scalar_product_simple_zero_vectors), 
        // TestTask("inner product zero vectors", test_scalar_product_std_zero_vectors),
        // TestTask("scalar product one", test_scalar_product_simple_one),
        // TestTask("inner product one", test_scalar_product_std_one),
        // TestTask("scalar product universal", test_scalar_product_universal),
        TestTask("normalize vector", FunctionOptimizationType::kRow, AlgebraObjectVersion::kEmpty, test_normalize_vector),
        TestTask("throws when lengths of vectors are unequal", FunctionOptimizationType::kRow, AlgebraObjectVersion::kEmpty, test_scalar_product_simple_different_length_of_vectors),
        };
    fill_tasks(tasks);
    TestRunner test_runner(&tasks);
    test_runner.run_all(std::cout);
    return 0;
}
