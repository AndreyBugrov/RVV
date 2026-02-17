#include "vector_operations.hpp"

num_type get_vector_norm(const vector<num_type>& vec){
    return get_vector_norm(vec.data(), vec.size());
}

num_type get_vector_norm(const num_type* vec, size_t length){
    num_type square_norm = inner_simple_dot_product(vec, vec, length);
    return sqrt(square_norm);
}

void normalize_vector_inplace(vector<num_type>& vec, num_type norm){
    normalize_vector_inplace(vec.data(), norm, vec.size());
}

void normalize_vector_inplace(num_type* vec, num_type norm, size_t length){
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

void sub_vector_from_vector_inplace_vector(vector<num_type>& minuend, const vector<num_type>& subtrahend){
    sub_vector_from_vector_inplace(minuend.data(), subtrahend.data(), minuend.size());
}

void sub_vector_from_vector_inplace(num_type* minuend, const num_type* subtrahend, size_t length){
    for(size_t i=0;i<length;++i){
        minuend[i]-=subtrahend[i];
    }
}

void sub_vector_from_vector_inplace_simd(num_type* minuend, const num_type* subtrahend, size_t length){
    #pragma omp simd
    for(size_t i=0;i<length;++i){
        minuend[i]-=subtrahend[i];
    }
}

void sub_vector_from_vector_inplace_unrolling(num_type* minuend, const num_type* subtrahend, size_t length){
    for(size_t i=0;i<length;i+=4){
        minuend[i] -= subtrahend[i];
        minuend[i+1] -= subtrahend[i+1];
        minuend[i+2] -= subtrahend[i+2];
        minuend[i+3] -= subtrahend[i+3];
    }
}

vector<num_type> multiply_vector_by_number(const vector<num_type>& vec, num_type number){
    vector<num_type> multiplied(vec);
    for(size_t i=0;i<vec.size();++i){
        multiplied[i]*=number;
    }
    return multiplied;
}

void inner_multiply_vector_by_number(const num_type* vec, num_type* mutiplied_vec, num_type number, size_t length){
    for(size_t i=0;i<length;++i){
        mutiplied_vec[i] = vec[i] * number;
    }
}

void inner_multiply_vector_by_number_simd(const num_type* vec, num_type* mutiplied_vec, num_type number, size_t length){
    #pragma omp simd
    for(size_t i=0; i<length; ++i){
        mutiplied_vec[i] = vec[i] * number;
    }
}

void inner_multiply_vector_by_number_unrolling(const num_type* vec, num_type* mutiplied_vec, num_type number, size_t length){
    for(size_t i=0;i<length;i+=kUnrollCoefficient){
        mutiplied_vec[i] = vec[i] * number;
        mutiplied_vec[i+1] = vec[i+1] * number;
        mutiplied_vec[i+2] = vec[i+2] * number;
        mutiplied_vec[i+3] = vec[i+3] * number;
    }
}

void inner_multiply_vector_by_number_unrolling_par(const num_type* __restrict__ vec, num_type* __restrict__ multiplied_vec, num_type number, size_t length){
    #pragma omp parallel for shared(multiplied_vec, number, length, kUnrollCoefficient)
    for(size_t i=0;i<length;i+=kUnrollCoefficient){
        multiplied_vec[i] = vec[i] * number;
        multiplied_vec[i+1] = vec[i+1] * number;
        multiplied_vec[i+2] = vec[i+2] * number;
        multiplied_vec[i+3] = vec[i+3] * number;
    }
}

void inner_element_wise_multiply_vector_by_vector_unrolling(const num_type* a, const num_type* b, num_type* result, size_t length){
    for(size_t i=0;i<length;i+=kUnrollCoefficient){
        result[i] = a[i] * b[i];
        result[i+1] = a[i+1] * b[i+1];
        result[i+2] = a[i+2] * b[i+2];
        result[i+3] = a[i+3] * b[i+3];
    }
}

void matrix_multiply_vector_by_number_optimal(const num_type* vec, num_type* mutiplied_vec, num_type number, size_t length){
    num_type tmp_vec[kUnrollCoefficient];
    for(size_t i=0;i<length;i+=kUnrollCoefficient){
        tmp_vec[0] = vec[i] * number;
        tmp_vec[1] = vec[i+1] * number;
        tmp_vec[2] = vec[i+2] * number;
        tmp_vec[3] = vec[i+3] * number;

        mutiplied_vec[i] += tmp_vec[0];
        mutiplied_vec[i+1] += tmp_vec[1];
        mutiplied_vec[i+2] += tmp_vec[2];
        mutiplied_vec[i+3] += tmp_vec[3];
    }
}
