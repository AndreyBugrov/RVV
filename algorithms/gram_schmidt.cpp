#include "gram_schmidt.hpp"


vector<num_type> vector_proj(const vector<num_type>& projected, const vector<num_type>& mapped_vec){
    num_type a_b = dot_product_simple_unsafe(projected, mapped_vec, projected.size());
    num_type b_b = dot_product_simple_unsafe(mapped_vec, mapped_vec, mapped_vec.size());
    return multiply_vector_by_number(mapped_vec, a_b/b_b);
}

void proj(const num_type* projected, const num_type* mapped_vec, num_type* projection, size_t length){
    num_type a_b = inner_simple_dot_product(projected, mapped_vec, length);
    num_type b_b = inner_simple_dot_product(mapped_vec, mapped_vec, length);
    inner_multiply_vector_by_number(mapped_vec, projection, a_b/b_b, length);
}

void proj_simd(const num_type* projected, const num_type* mapped_vec, num_type* projection, size_t length){
    num_type a_b = inner_dot_product_simd(projected, mapped_vec, length);
    num_type b_b = inner_dot_product_simd(mapped_vec, mapped_vec, length);
    inner_multiply_vector_by_number_simd(mapped_vec, projection, a_b/b_b, length);
}

void proj_unrolling(const num_type* projected, const num_type* mapped_vec, num_type* projection, size_t length){
    num_type a_b = inner_dot_product_unrolling(projected, mapped_vec, length);
    num_type b_b = inner_dot_product_unrolling(mapped_vec, mapped_vec, length);
    inner_multiply_vector_by_number_unrolling(mapped_vec, projection, a_b/b_b, length);
}

vector<vector<num_type>> gram_schmidt_base_simple(const vector<vector<num_type>>& vec_system){
    if(vec_system.size()==0){
        return vec_system;
    }
    if (vec_system.size() > vec_system[0].size()){
        throw Exception(ErrorType::kIncorrectLengthRatio, generate_string("Vector system size (", vec_system.size(), ") is more than vector length (", vec_system[0].size(), ")"));
    }
    size_t vec_system_size = vec_system.size();
    vector<vector<num_type>> orthogonal_system(vec_system.size());
    for(size_t vec_index=0;vec_index<vec_system_size;++vec_index){
        orthogonal_system[vec_index] = vec_system[vec_index];
        for(size_t proj_index=0;proj_index<vec_index;++proj_index){
            vector<num_type> projection = vector_proj(vec_system[vec_index], orthogonal_system[proj_index]);
            sub_vector_from_vector_inplace_vector(orthogonal_system[vec_index], projection);
        }
    }
    return orthogonal_system;
}

vector_num gram_schmidt_matrix_simple(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    return gram_schmidt_matrix_common(transposed_matrix, row_count, column_count, proj, sub_vector_from_vector_inplace);
}

vector_num gram_schmidt_matrix_simd(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    // applying sub_vector_from_vector_inplace_simd instead of sub_vector_from_vector_inplace leads to little slowdown
    return gram_schmidt_matrix_common(transposed_matrix, row_count, column_count, proj_simd, sub_vector_from_vector_inplace);
}

vector_num gram_schmidt_matrix_unrolling(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    // applying sub_vector_from_vector_inplace_simd instead of sub_vector_from_vector_inplace does not lead to any change
    // autovectorization is not applied
    return gram_schmidt_matrix_common(transposed_matrix, row_count, column_count, proj_unrolling, sub_vector_from_vector_inplace);
}

vector_num gram_schmidt_matrix_common(vector_num& transposed_matrix, size_t row_count, size_t column_count, proj_function proj_foo, sub_function sub_foo){
    check_matrix(transposed_matrix, row_count, column_count);
    vector_num orthogonal_matrix = transposed_matrix;
    for(size_t vec_index=0;vec_index<row_count;++vec_index){
        for(size_t proj_index=0;proj_index<vec_index;++proj_index){
            num_type* projection = new num_type[column_count];
            proj_foo(&transposed_matrix[vec_index*column_count], &orthogonal_matrix[proj_index*column_count], projection, column_count);
            sub_foo(&orthogonal_matrix[vec_index*column_count], projection, column_count);
            delete[] projection;
        }
    }
    return orthogonal_matrix;
}

vector_num gram_schmidt_matrix_inline_common(vector_num& transposed_matrix, size_t row_count, size_t column_count, dot_product_function dot_foo, sub_function sub_foo, number_mult_function mult_foo, bool enable_parallel=false){
    check_matrix(transposed_matrix, row_count, column_count);
    vector_num orthogonal_matrix = transposed_matrix;
    vector_num dot_product_results(row_count);
    size_t vec_index, proj_index;
    #pragma omp parallel for shared(orthogonal_matrix, dot_product_results, row_count, column_count, dot_foo, sub_foo, mult_foo) private(vec_index, proj_index) if (enable_parallel)
    for(vec_index=1;vec_index<row_count;++vec_index){
        dot_product_results[vec_index-1] = dot_foo(&orthogonal_matrix[(vec_index-1)*column_count], &orthogonal_matrix[(vec_index-1)*column_count], column_count);
        for(proj_index=0;proj_index<vec_index;++proj_index){
            num_type* projection = new num_type[column_count];
            num_type multiplier = dot_foo(&transposed_matrix[vec_index*column_count],  &orthogonal_matrix[proj_index*column_count], column_count);
            mult_foo(&orthogonal_matrix[proj_index*column_count], projection, multiplier/dot_product_results[proj_index], column_count);
            sub_foo(&orthogonal_matrix[vec_index*column_count], projection, column_count);
            delete[] projection;
        }
    }
    return orthogonal_matrix;
}


vector_num gram_schmidt_matrix_inline(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    return gram_schmidt_matrix_inline_common(transposed_matrix, row_count, column_count, inner_dot_product_unrolling, sub_vector_from_vector_inplace, inner_multiply_vector_by_number_unrolling);
}

vector_num gram_schmidt_matrix_inline_par(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    return gram_schmidt_matrix_inline_common(transposed_matrix, row_count, column_count, inner_dot_product_unrolling, sub_vector_from_vector_inplace, inner_multiply_vector_by_number_unrolling, true);
}

void vector_matrix_product(const num_type* vec, const num_type* transposed_matrix, num_type* result_vec, size_t row_count, size_t column_count){  
    for(size_t vec_index = 0; vec_index < row_count; ++vec_index){
        result_vec[vec_index] = inner_dot_product_unrolling(&transposed_matrix[vec_index*column_count], vec, column_count);
    }
}

vector_num gram_schmidt_full_matrix(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    check_matrix(transposed_matrix, row_count, column_count);
    vector_num orthogonal_matrix = transposed_matrix;
    vector_num square_ort_reciprocals(row_count);
    vector_num vector_matrix_products(row_count);
    vector_num multipliers(row_count);
    for(size_t vec_index=1;vec_index<row_count;++vec_index){
        square_ort_reciprocals[vec_index-1] = 1.0 / inner_dot_product_unrolling(&orthogonal_matrix[(vec_index-1)*column_count], &orthogonal_matrix[(vec_index-1)*column_count], column_count);
        vector_matrix_product(&transposed_matrix[vec_index * column_count], orthogonal_matrix.data(), vector_matrix_products.data(), vec_index, column_count);
        // inner_element_wise_multiply_vector_by_vector_unrolling(vector_matrix_products.data(), square_ort_reciprocals.data(), multipliers.data(), vec_index);
        for(size_t i = 0; i < vec_index; i++){
            multipliers[i] = vector_matrix_products[i] * square_ort_reciprocals[i];
        }
        
        for(size_t proj_index=0;proj_index<vec_index;++proj_index){
            num_type* projection = new num_type[column_count];
            inner_multiply_vector_by_number_unrolling(&orthogonal_matrix[proj_index*column_count], projection, multipliers[proj_index], column_count);
            sub_vector_from_vector_inplace(&orthogonal_matrix[vec_index*column_count], projection, column_count);
            delete[] projection;
        }
    }
    return orthogonal_matrix;
}

void check_matrix(vector_num& transposed_matrix, size_t row_count, size_t column_count){
    if (transposed_matrix.size() != row_count * column_count){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Vector system matrix size (", transposed_matrix.size(), ") is not equal to given length (", row_count, " * ", column_count, ")"));
    }
    if (row_count > column_count){
        throw Exception(ErrorType::kIncorrectLengthRatio, generate_string("Matrix row count (", row_count, ") is more than matrix column count (", column_count, ")"));
    }
}
