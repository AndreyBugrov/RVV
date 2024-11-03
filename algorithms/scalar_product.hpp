#pragma once

#include <vector> // std::vector
#include <numeric> // std::inner_product
#include <asm/unistd.h> // intrinsics

#include "../common/exception.hpp"  // Exception
#include "../common/generators.hpp"  // generate_string

using std::vector;

typedef double num_type;  // to replace by int_64t or other

struct VectorProdInput{
    vector<double> a;
    vector<double> b;
    size_t length;

    VectorProdInput(const vector<double>& vec_1, const vector<double>& vec_2, size_t length_input): a(vec_1), b(vec_2), length(length_input){}
    VectorProdInput(const VectorProdInput& other): a(other.a), b(other.b), length(other.length){}
    ~VectorProdInput() = default;
};

inline void check_lengths_are_equal(size_t a_size, size_t b_size);

double scalar_product_simple(const VectorProdInput& input);

double scalar_product_std(const VectorProdInput& input);

double scalar_product_std(vector<double> a, vector<double> b, size_t length);

double scalar_product_std_unsafe(vector<double> a, vector<double> b, size_t length);

double scalar_product_boost(const VectorProdInput& input);

double scalar_product_intrinsic(const VectorProdInput& input);
