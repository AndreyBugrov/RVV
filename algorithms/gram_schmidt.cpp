#include "gram_schmidt.hpp"


vector<num_type> proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec){
    num_type a_b = scalar_product_opt_unsafe(projected, mapped_vec, projected.size());
    num_type b_b = scalar_product_opt_unsafe(mapped_vec, mapped_vec, mapped_vec.size());
    return multiply_vector_by_number(mapped_vec, a_b/b_b);
}

vector<vector<num_type>> gram_schmidt_base_simple(vector<vector<num_type>>& vec_system){
    if(vec_system.size()==0){
        return vec_system;
    }
    size_t vec_system_size = vec_system.size();
    vector<vector<num_type>> orthogonal_system(vec_system.size());
    for(size_t vec_num=0;vec_num<vec_system_size;++vec_num){
        orthogonal_system[vec_num] = vec_system[vec_num];
        for(size_t proj_num=0;proj_num<vec_num;++proj_num){
            vector<num_type> projection = proj(vec_system[vec_num], orthogonal_system[proj_num]);
            sub_vector_from_vector_inplace(orthogonal_system[vec_num], projection);
        }
    }
    return orthogonal_system;
}