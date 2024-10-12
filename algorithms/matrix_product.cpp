#include "matrix_product.hpp"

static void check_length(size_t a_size, size_t b_size, size_t c_size, size_t a_row_num, size_t a_column_num, size_t b_column_num){
    if(a_size != a_row_num * a_column_num){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("1st matrix size (", a_size, ") is not equal to given length (", a_row_num * a_column_num, ")"));
    }
    if(b_size != a_column_num*b_column_num){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("2nd matrix size (", b_size, ") is not equal to given length (", a_column_num*b_column_num, ")"));
    }
    if(c_size != a_row_num*b_column_num){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Result matrix size (", c_size, ") is not equal to given length (", a_row_num*b_column_num, ")"));
    }
}

void matrix_prod_base_seq(MatrixProdInput& input){
    check_length(input.a.size(), input.b.size(), input.c.size(), input.a_row_num, input.a_column_num, input.b_column_num);
    for(size_t i=0;i<input.a_row_num;++i){
        for(size_t j=0;j<input.a_column_num;++j){
            for(size_t k=0;k<input.b_column_num;++k){
                
            }
        }
    }
}

void matrix_prod_row_seq(MatrixProdInput& input){
    check_length(input.a.size(), input.b.size(), input.c.size(), input.a_row_num, input.a_column_num, input.b_column_num);
    for(int i=0;i<row_length;i++){
        for(int j=0;j<row_length;j++){
            for(int k=0;k<row_length;k++){
                c[i*row_length+k]+=a[i*row_length+j]*b[j*row_length+k];
            }
        }
    }
}