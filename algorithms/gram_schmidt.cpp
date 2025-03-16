#include "gram_schmidt.hpp"


vector<num_type> proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec){
    num_type a_b = dot_product_opt(projected, mapped_vec, projected.size());
    num_type b_b = dot_product_opt(mapped_vec, mapped_vec, mapped_vec.size());
    return multiply_vector_by_number(mapped_vec, a_b/b_b);
}

num_type* proj(const num_type* projected, const num_type* mapped_vec, size_t length){
    num_type a_b = inner_optimal_dot_product(projected, mapped_vec, length);
    num_type b_b = inner_optimal_dot_product(mapped_vec, mapped_vec, length);
    return inner_multiply_vector_by_number(mapped_vec, a_b/b_b, length);
}

vector<vector<num_type>> gram_schmidt_base_simple(const vector<vector<num_type>>& vec_system){
    if(vec_system.size()==0){
        return vec_system;
    }
    size_t vec_system_size = vec_system.size();
    vector<vector<num_type>> orthogonal_system(vec_system.size());
    for(size_t vec_index=0;vec_index<vec_system_size;++vec_index){
        orthogonal_system[vec_index] = vec_system[vec_index];
        for(size_t proj_index=0;proj_index<vec_index;++proj_index){
            vector<num_type> projection = proj(vec_system[vec_index], orthogonal_system[proj_index]);
            sub_vector_from_vector_inplace(orthogonal_system[vec_index], projection);
        }
    }
    return orthogonal_system;
}

vector_num gram_schmidt_matrix_simple_inplace(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    if(row_count == 0 || column_count == 0){
        return vector_num(0);
    }
    vector_num orthogonal_matrix = transposed_matrix;
    for(size_t vec_index=0;vec_index<row_count;++vec_index){
        for(size_t proj_index=0;proj_index<vec_index;++proj_index){
            num_type* projection = proj(&transposed_matrix[vec_index*column_count], &orthogonal_matrix[proj_index*column_count], column_count);
            inner_sub_vector_from_vector_inplace(&orthogonal_matrix[vec_index*column_count], projection, column_count);
        }
    }
    return orthogonal_matrix;
}

// vector<vector_num> gram_schmidt_modified_simple(const vector<vector_num>& vec_system){
//     if(vec_system.size()==0){
//         return vec_system;
//     }
//     size_t vec_system_size = vec_system.size();
//     vector<vector<num_type>> orthogonal_system(vec_system.size());
//     for(size_t vec_index=0;vec_index<vec_system_size;++vec_index){
//         orthogonal_system[vec_index] = vec_system[vec_index];
//         for(size_t proj_index=0;proj_index<vec_index;++proj_index){
//             vector<num_type> projection = proj(vec_system[vec_index], orthogonal_system[proj_index]);
//             sub_vector_from_vector_inplace(orthogonal_system[vec_index], projection);
//         }
//     }
//     return orthogonal_system;
// }
