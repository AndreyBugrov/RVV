#pragma once

#include <vector> // std::vector
#include <numeric> // std::inner_product
#include <asm/unistd.h> // architecture 

// #include "../third_party/boost_1_86_0/boost/numeric/ublas/vector.hpp"
// #include "../third_party/boost_1_86_0/boost/numeric/ublas/io.hpp"

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

inline void check_lengths_are_equal(size_t a_size, size_t b_size);

double scalar_product_simple(const VectorProdInput& input);

double scalar_product_std(const VectorProdInput& input);

double scalar_product_boost(const VectorProdInput& input);

double scalar_product_intrinsic(const VectorProdInput& input);
