#pragma once

#include <random>  // random vector generation
#include <cstring> // memset in zero matrix generating
#include <string>  // string generator
#include <sstream>  // ostringstream

void generate_rand_array(double* arr, size_t n, double min, double max);
int generate_rand_number(int min, size_t max);
void generate_zero_array(double* arr, size_t n);
void generate_identity_matrix(double* matrix, size_t row_num, size_t column_num);

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
