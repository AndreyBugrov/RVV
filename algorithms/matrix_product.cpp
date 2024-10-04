#include "matrix_product.hpp"

void matrix_prod_row_seq(const vector<double>& a, const vector<double>& b, vector<double>& c, int row_length){
    for(int i=0;i<row_length;i++){
        for(int j=0;j<row_length;j++){
            for(int k=0;k<row_length;k++){
                c[i*row_length+k]+=a[i*row_length+j]*b[j*row_length+k];
            }
        }
    }
}