#pragma once

#include <iostream> // cerr

#include <string>
#include <vector>
#include <map>

#include "experiment_task.hpp"  // run_experiment_task

#include "../common/exception.hpp"
#include "../common/generators.hpp"  // only to generate error message

#include "../algorithms/scalar_product.hpp"
#include "../algorithms/matrix_product.hpp"
#include "../algorithms/gram_schmidt.hpp"
#include "../algorithms/qr_decomposition.hpp"

enum class FunctionIndex{
    kScalarProductSimple,
    kScalarProductStd,
    kMatrixProductSimple,
    kGramSchmidtSimple,
    kQRSimple,
};

BaseTaskOutput run_experiment(std::string function_name, std::vector<size_t> function_arguments);

void print_experiment_result(const BaseTaskOutput& output, std::ostream& stream);
