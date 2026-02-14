#include "outer_product.hpp"

std::vector<num_type> outer_product_opt(const vector<num_type>& a, const vector<num_type>& b, size_t a_length, size_t b_length){
    std::vector<num_type> result(a_length * b_length);
    for(size_t a_element_index = 0; a_element_index < a_length; ++a_element_index){
        for(size_t b_element_index = 0; b_element_index < b_length; ++b_element_index){
            result[a_element_index*b_length+b_element_index] = a[a_element_index]*b[b_element_index];
        }
    }
    return result;
}
