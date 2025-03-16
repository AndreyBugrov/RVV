#include "matrix_product.hpp"

static void check_length(size_t a_size, size_t b_size, size_t c_size, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    if(a_size != a_row_count * a_column_count){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("1st matrix size (", a_size, ") is not equal to given length (", a_row_count, " * ", a_column_count, ")"));
    }
    if(b_size != a_column_count*b_column_count){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("2nd matrix size (", b_size, ") is not equal to given length (", a_column_count, " * ", b_column_count, ")"));
    }
    if(c_size != a_row_count*b_column_count){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Result matrix size (", c_size, ") is not equal to given length (", a_row_count, " * ", b_column_count, ")"));
    }
}

void matrix_product_base_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
    for(size_t i=0;i<a_row_count;++i){
        for(size_t j=0;j<b_column_count;++j){
            for(size_t k=0;k<a_column_count;++k){
                c[i*b_column_count+j]+=a[i*a_column_count+k]*b[k*b_column_count+j];
            }
        }
    }
}

void matrix_product_second_transposed_simple(const vector<num_type>& a, const vector<num_type>& b_T, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    check_length(a.size(), b_T.size(), c.size(), a_row_count, a_column_count, b_column_count);
    for(size_t i=0;i<a_row_count;++i){
        for(size_t j=0;j<b_column_count;++j){
            for(size_t k=0;k<a_column_count;++k){
                c[i*b_column_count+k]+=a[i*a_column_count+k]*b_T[j*a_column_count+k];
            }
        }
    }
}

void matrix_product_row_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
    for(size_t i=0;i<a_row_count;++i){
        for(size_t j=0;j<a_column_count;++j){
            for(size_t k=0;k<b_column_count;++k){
                c[i*b_column_count+k]+=a[i*a_column_count+j]*b[j*b_column_count+k];
            }
        }
    }
}
