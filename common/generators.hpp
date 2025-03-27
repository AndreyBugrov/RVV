#pragma once

#include <random>  // random vector generation
#include <cstring> // memset in zero matrix generating
#include <string>  // string generator
#include <sstream>  // ostringstream

#include "defines.hpp"  // num_type

void generate_rand_array(num_type* arr, size_t arr_size, num_type min, num_type max);
size_t generate_rand_length(size_t min, size_t max);
void generate_zero_array(num_type* arr, size_t arr_size);

void generate_identity_matrix(num_type* matrix, size_t row_num, size_t column_num);

num_type generate_rand_value(num_type min, num_type max);

template <typename T>
static void inner_string_generator(std::ostream& o, T t)
{
    o << t;
}

template<typename T, typename... Args>
static void inner_string_generator(std::ostream& o, T t, Args... args) // recursive variadic function
{
    inner_string_generator(o, t);
    inner_string_generator(o, args...);
}

template<typename... Args>
std::string generate_string(Args... args)
{
    std::ostringstream oss;
    inner_string_generator(oss, args...);
    return oss.str();
}
