#pragma once

#include <vector> // std::vector
#include <numeric> // std::inner_product
#include <asm/unistd.h> // intrinsics

#include "../common/defines.hpp"  // num_type
#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string

using std::vector;

struct VectorProdInput{
    vector<double> a;
    vector<double> b;
    size_t length;

    VectorProdInput(const vector<double>& vec_1, const vector<double>& vec_2, size_t length_input): a(vec_1), b(vec_2), length(length_input){}
    VectorProdInput(const VectorProdInput& other): a(other.a), b(other.b), length(other.length){}
    ~VectorProdInput() = default;
};

double scalar_product_simple(const VectorProdInput& input);

double scalar_product_std(const VectorProdInput& input);

double scalar_product_std(vector<double> a, vector<double> b, size_t length);

num_type scalar_product_std_unsafe(vector<num_type> a, vector<num_type> b, size_t length);

num_type scalar_product_intrinsic(vector<num_type> a, vector<num_type> b, size_t length);
