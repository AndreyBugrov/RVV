#pragma once
#include <vector>

// I can't check QR decomposition using any integer type
// due to the calculation of the vector norm and division by it
using num_type = double;
using vector_num = std::vector<num_type>;