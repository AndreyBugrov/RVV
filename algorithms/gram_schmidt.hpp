#pragma once

#include <vector>  // matrices

#include "vector_opeations.hpp"  // vector multiplication from scalar product, mult by number, inplace vector minus

using std::vector;

vector<num_type> proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec);

vector<vector<num_type>> gram_schmidt_base_simple(vector<vector<num_type>>& vec_system);
