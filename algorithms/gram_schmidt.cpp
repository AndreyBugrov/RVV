#include "gram_schmidt.hpp"


vector<num_type> proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec){
    num_type a_b = dot_product_simple_unsafe(projected, mapped_vec, projected.size());
    num_type b_b = dot_product_simple_unsafe(mapped_vec, mapped_vec, mapped_vec.size());
    return multiply_vector_by_number(mapped_vec, a_b/b_b);
}

num_type* proj(const num_type* projected, const num_type* mapped_vec, size_t length){
    num_type a_b = inner_simple_dot_product(projected, mapped_vec, length);
    num_type b_b = inner_simple_dot_product(mapped_vec, mapped_vec, length);
    return inner_multiply_vector_by_number(mapped_vec, a_b/b_b, length);
}

num_type* proj_simd(const num_type* projected, const num_type* mapped_vec, size_t length){
    num_type a_b = inner_dot_product_simd(projected, mapped_vec, length);
    num_type b_b = inner_dot_product_simd(mapped_vec, mapped_vec, length);
    return inner_multiply_vector_by_number(mapped_vec, a_b/b_b, length);
}

num_type* proj_unrolling(const num_type* projected, const num_type* mapped_vec, size_t length){
    num_type a_b = inner_dot_product_unrolling(projected, mapped_vec, length);
    num_type b_b = inner_dot_product_unrolling(mapped_vec, mapped_vec, length);
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

vector_num gram_schmidt_matrix_simple(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    check_matrix(transposed_matrix, row_count, column_count);
    vector_num orthogonal_matrix = transposed_matrix;
    for(size_t vec_index=0;vec_index<row_count;++vec_index){
        for(size_t proj_index=0;proj_index<vec_index;++proj_index){
            num_type* projection = proj(&transposed_matrix[vec_index*column_count], &orthogonal_matrix[proj_index*column_count], column_count);
            sub_vector_from_vector_inplace(&orthogonal_matrix[vec_index*column_count], projection, column_count);
            delete projection;
        }
    }
    return orthogonal_matrix;
}

vector_num gram_schmidt_matrix_simd(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    check_matrix(transposed_matrix, row_count, column_count);
    vector_num orthogonal_matrix = transposed_matrix;
    for(size_t vec_index=0;vec_index<row_count;++vec_index){
        for(size_t proj_index=0;proj_index<vec_index;++proj_index){
            num_type* projection = proj_simd(&transposed_matrix[vec_index*column_count], &orthogonal_matrix[proj_index*column_count], column_count);
            sub_vector_from_vector_inplace(&orthogonal_matrix[vec_index*column_count], projection, column_count);
            delete projection;
        }
    }
    return orthogonal_matrix;
}

vector_num gram_schmidt_matrix_unrolling(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    check_matrix(transposed_matrix, row_count, column_count);
    vector_num orthogonal_matrix = transposed_matrix;
    for(size_t vec_index=0;vec_index<row_count;++vec_index){
        for(size_t proj_index=0;proj_index<vec_index;++proj_index){
            num_type* projection = proj(&transposed_matrix[vec_index*column_count], &orthogonal_matrix[proj_index*column_count], column_count);
            sub_vector_from_vector_inplace(&orthogonal_matrix[vec_index*column_count], projection, column_count);
            delete projection;
        }
    }
    return orthogonal_matrix;
}

void check_matrix(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    if (transposed_matrix.size() != row_count*column_count){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Vector system matrix size (", transposed_matrix.size(), ") is not equal to given length (", row_count, " * ", column_count, ")"));
    }
    if(row_count < column_count){
        throw Exception(ErrorType::kIncorrectLengthRatio, generate_string("Row count (", row_count, ") must be less or equal than column count (", column_count, ")"));
    }
}