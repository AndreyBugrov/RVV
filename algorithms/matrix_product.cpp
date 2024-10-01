#include "matr_mult.hpp"

void matr_mult_row(const vector<double>& a, const vector<double>& b, const vector<double>& c, int row_length){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                c[i*n+k]+=a[i*n+j]*b[j*n+k];
            }
        }
    }
}