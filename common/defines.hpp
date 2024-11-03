#pragma once

// I can't check QR decomposition using any integer type
// due to the calculation of the vector norm and division by it
typedef double num_type;  