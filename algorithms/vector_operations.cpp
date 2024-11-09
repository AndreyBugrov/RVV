#include "vector_opeations.hpp"

num_type get_vector_norm(const vector<num_type>& vec){
    num_type square_norm = scalar_product_std_unsafe(vec, vec, vec.size());
    return sqrt(square_norm);
}

void normalize_vector_inplace(vector<num_type>& vec, num_type norm){
    if(norm==0){
        return;
    }
    for(size_t coord_num=0;coord_num<vec.size();++coord_num){
        vec[coord_num] /= norm;
    }
}

bool is_vector_zero(const vector<num_type>& vec){
    for(size_t i=0;i<vec.size();++i){
        if(vec[i]!=0){
            return false;
        }
    }
    return true;
}

void sub_vector_from_vector_inplace(vector<num_type>& minuend, const vector<num_type>& subtrahend){
    for(size_t i=0;i<minuend.size();++i){
        minuend[i]-=subtrahend[i];
    }
}

vector<num_type> multiply_vector_by_number(const vector<num_type>& vec, num_type number){
    vector<num_type> multiplied(vec);
    for(size_t i=0;i<vec.size();++i){
        multiplied[i]*=number;
    }
    return multiplied;
}
