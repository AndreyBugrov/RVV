#include "gram_schmidt.hpp"

num_type get_vector_norm(const vector<num_type>& vec){
    double square_norm = scalar_product_std_unsafe(vec, vec, vec.size());
    return sqrt(square_norm);
}

void normalize_vector(vector<num_type>& vec, num_type norm){
    if(norm==0){
        return;
    }
    for(size_t coord_num=0;coord_num<vec.size();++coord_num){
        vec[coord_num] /= norm;
    }
}

void sub_vector_from_vector_inplace(vector<num_type>& minuend, const vector<num_type>& subtrahend){
    for(size_t i=0;i<minuend.size();++i){
        minuend[i]-=subtrahend[i];
    }
}

vector<vector<num_type>> gram_shmidt_base_simple(vector<vector<num_type>>& vec_system){
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