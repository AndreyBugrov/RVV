#pragma once

#include <random>  // random vector generation
#include <cstring> // memset in zero matrix generating

void generate_rand_array(double* arr, int n, double min, double max);
int generate_rand_number(int min, int max);
void generate_zero_array(double* arr, int n);