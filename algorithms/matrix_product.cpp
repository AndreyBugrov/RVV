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

void matrix_product_row_scalar(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);

    for(size_t i=0;i<a_row_count;++i){
        for(size_t j=0;j<a_column_count;++j){
            matrix_multiply_vector_by_number_optimal(b.data() + j * b_column_count, c.data() + i * b_column_count, a[i*a_column_count+j], b_column_count);
        }
    }
}

void matrix_product_row_block(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
    for(size_t ik = 0; ik < a_row_count; ik += kBlockSize){
        for(size_t jk = 0; jk < a_column_count; jk += kBlockSize){
            for(size_t kk = 0; kk < b_column_count; kk += kBlockSize){
                for(size_t i = 0; i < kBlockSize; ++i){
                    for(size_t j = 0; j < kBlockSize; ++j){
                        for(size_t k = 0; k < kBlockSize; ++k){
                            c[(ik + i) * b_column_count + (kk + k)] += a[(ik + i) * a_column_count + (jk + j)] * b[(jk + j) * b_column_count + (kk + k)];
                        }
                    }
                }
            }
        }
    }
}

void matrix_product_row_block_scalar(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
    for(size_t ik = 0; ik < a_row_count; ik += kBlockSize){
        for(size_t jk = 0; jk < a_column_count; jk += kBlockSize){
            for(size_t kk = 0; kk < b_column_count; kk += kBlockSize){
                for(size_t i = 0; i < kBlockSize; ++i){
                    for(size_t j = 0; j < kBlockSize; ++j){
                        matrix_multiply_vector_by_number_optimal(b.data() + (jk + j) * b_column_count + (kk), c.data() + (ik + i) * b_column_count + (kk), a[(ik + i) * a_column_count + (jk + j)], kBlockSize);
                    }
                }  
            }
        }
    }
}

// void matrix_product_row_block_scalar_par(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
//     check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
//     size_t ik, jk, kk, i, j;
//     #pragma omp parallel for shared(a, b, c, a_row_count, a_column_count, b_column_count, kBlockSize) private(ik, jk, kk, i, j)
//     for(ik = 0; ik < a_row_count; ik += kBlockSize){
//         for(jk = 0; jk < a_column_count; jk += kBlockSize){
//             for(kk = 0; kk < b_column_count; kk += kBlockSize){
//                 for(i = 0; i < kBlockSize; ++i){
//                     for(j = 0; j < kBlockSize; ++j){
//                         matrix_multiply_vector_by_number_optimal(b.data() + (jk + j) * b_column_count + (kk), c.data() + (ik + i) * b_column_count + (kk), a[(ik + i) * a_column_count + (jk + j)], kBlockSize);
//                     }
//                 }  
//             }
//         }
//     }
// }

// void matrix_product_row_block_scalar_par(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
//     check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
//     size_t ik, jk, kk, i, j, k;
//     #pragma omp parallel for shared(a, b, c, a_row_count, a_column_count, b_column_count, kBlockSize) private(ik, jk, kk, i, j, k)
//     for(ik = 0; ik < a_row_count; ik += kBlockSize){
//         for(jk = 0; jk < a_column_count; jk += kBlockSize){
//             for(kk = 0; kk < b_column_count; kk += kBlockSize){
//                 for(i = 0; i < kBlockSize; ++i){
//                     for(j = 0; j < kBlockSize; ++j){
//                         for(k = 0; k < kBlockSize; ++k){
//                             c[(ik + i) * b_column_count + (kk + k)] += a[(ik + i) * a_column_count + (jk + j)] * b[(jk + j) * b_column_count + (kk + k)];
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

void matrix_product_row_block_scalar_par(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
    size_t ik, jk, kk, i, j;
    #pragma omp parallel for shared(a, b, c, a_row_count, a_column_count, b_column_count, kBlockSize) private(ik, jk, kk, i, j)
    for(ik = 0; ik < a_row_count; ik += kBlockSize){
        for(jk = 0; jk < a_column_count; jk += kBlockSize){
            for(kk = 0; kk < b_column_count; kk += kBlockSize){
                for(i = 0; i < kBlockSize; ++i){
                    for(j = 0; j < kBlockSize; ++j){
                        for(size_t i=0;i<kBlockSize;i+=kUnrollCoefficient){
                            c[(ik + i) * b_column_count + (kk) + i] += b[(jk + j) * b_column_count + (kk) + i] * a[(ik + i) * a_column_count + (jk + j)];
                            c[(ik + i) * b_column_count + (kk) + i + 1] += b[(jk + j) * b_column_count + (kk) + i + 1] * a[(ik + i) * a_column_count + (jk + j)];
                            c[(ik + i) * b_column_count + (kk) + i + 2] += b[(jk + j) * b_column_count + (kk) + i + 2] * a[(ik + i) * a_column_count + (jk + j)];
                            c[(ik + i) * b_column_count + (kk) + i + 3] += b[(jk + j) * b_column_count + (kk) + i + 3] * a[(ik + i) * a_column_count + (jk + j)];
                        }
                        // matrix_multiply_vector_by_number_optimal(b.data() + (jk + j) * b_column_count + (kk), c.data() + (ik + i) * b_column_count + (kk), a[(ik + i) * a_column_count + (jk + j)], kBlockSize);
                        // for(k = 0; k < kBlockSize; ++k){
                        //     c[(ik + i) * b_column_count + (kk + k)] += a[(ik + i) * a_column_count + (jk + j)] * b[(jk + j) * b_column_count + (kk + k)];
                        // }
                    }
                }
            }
        }
    }
}
