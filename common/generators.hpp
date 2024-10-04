#pragma once

#include <random>  // random vector generation
#include <cstring> // memset in zero matrix generating
#include <string>  // string generator
#include <sstream>  // ostringstream

void generate_rand_array(double* arr, int n, double min, double max);
int generate_rand_number(int min, int max);
void generate_zero_array(double* arr, int n);

template <typename T>
static void inner_string_generator(std::ostream& o, T t)
{
    o << t << std::endl;
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
