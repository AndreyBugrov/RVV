#include "matrix_product.hpp"

static void check_length(size_t a_size, size_t b_size, size_t c_size, size_t a_row_num, size_t a_column_num, size_t b_column_num){
    if(a_size != a_row_num * a_column_num){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("1st matrix size (", a_size, ") is not equal to given length (", a_row_num, " * ", a_column_num, ")"));
    }
    if(b_size != a_column_num*b_column_num){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("2nd matrix size (", b_size, ") is not equal to given length (", a_column_num, " * ", b_column_num, ")"));
    }
    if(c_size != a_row_num*b_column_num){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Result matrix size (", c_size, ") is not equal to given length (", a_row_num, " * ", b_column_num, ")"));
    }
}

void matrix_prod_base_simple(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num){
    check_length(a.size(), b.size(), c.size(), a_row_num, a_column_num, b_column_num);
    for(size_t i=0;i<a_row_num;++i){
        for(size_t j=0;j<b_column_num;++j){
            for(size_t k=0;k<a_column_num;++k){
                c.at(i*b_column_num+j)+=a.at(i*a_column_num+k)*b.at(k*b_column_num+j);
            }
        }
    }
}

void matrix_prod_base_std(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num){
    check_length(a.size(), b.size(), c.size(), a_row_num, a_column_num, b_column_num);
    for(size_t i=0;i<a_row_num;++i){
        for(size_t j=0;j<a_column_num;++j){
            for(size_t k=0;k<b_column_num;++k){
                
            }
        }
    }
}

void matrix_prod_second_transposed_simple(const vector<num_type>& a, const vector<num_type>& b_T, vector<num_type>& c, size_t a_row_num, size_t a_column_num, size_t b_column_num){
    check_length(a.size(), b_T.size(), c.size(), a_row_num, a_column_num, b_column_num);
    for(size_t i=0;i<a_row_num;++i){
        for(size_t j=0;j<b_column_num;++j){
            for(size_t k=0;k<a_column_num;++k){
                c[i*b_column_num+k]+=a[i*a_column_num+k]*b_T[j*a_column_num+k];
            }
        }
    }
}
