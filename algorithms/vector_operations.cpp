#include "vector_operations.hpp"

num_type get_vector_norm(const vector<num_type>& vec){
    num_type square_norm = dot_product_opt(vec, vec, vec.size());
    return sqrt(square_norm);
}

num_type inner_get_vector_norm(const num_type* vec, size_t length){
    num_type square_norm = inner_optimal_dot_product(vec, vec, length);
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

void inner_normalize_vector_inplace(num_type* vec, num_type norm, size_t length){
    if(norm==0){
        return;
    }
    for(size_t coord_num=0;coord_num<length;++coord_num){
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

void inner_sub_vector_from_vector_inplace(num_type* minuend, const num_type* subtrahend, size_t length){
    for(size_t i=0;i<length;++i){
        minuend[i]-=subtrahend[i];
    }
}

void add_vector_inplace(vector<num_type>& base_vec, const vector<num_type>& other_vec){
    for(size_t i=0; i< base_vec.size();++i){
        base_vec[i] -= other_vec[i];
    }
}

vector<num_type> multiply_vector_by_number(const vector<num_type>& vec, num_type number){
    vector<num_type> multiplied(vec);
    for(size_t i=0;i<vec.size();++i){
        multiplied[i]*=number;
    }
    return multiplied;
}

num_type* inner_multiply_vector_by_number(const num_type* vec, num_type number, size_t length){
    num_type* multiplied = new num_type[length];
    memcpy(multiplied, vec, length*sizeof(num_type));
    for(size_t i=0;i<length;++i){
        multiplied[i]*=number;
    }
    return multiplied;
}
