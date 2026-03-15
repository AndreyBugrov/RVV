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

void matrix_product_row_block_unrolling(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
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

void matrix_product_row_block_par(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
    check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
    size_t ik, jk, kk, i, j, k;
    #pragma omp parallel for shared(a, b, c, a_row_count, a_column_count, b_column_count, kBlockSize) private(ik, jk, kk, i, j, k)
    for(ik = 0; ik < a_row_count; ik += kBlockSize){
        for(jk = 0; jk < a_column_count; jk += kBlockSize){
            for(kk = 0; kk < b_column_count; kk += kBlockSize){
                for(i = 0; i < kBlockSize; ++i){
                    for(j = 0; j < kBlockSize; ++j){
                        for(k = 0; k < kBlockSize; ++k){
                            c[(ik + i) * b_column_count + (kk + k)] += a[(ik + i) * a_column_count + (jk + j)] * b[(jk + j) * b_column_count + (kk + k)];
                        }
                    }
                }
            }
        }
    }
}

void product(num_type a_coeff, const num_type __restrict__* start_b_index, num_type __restrict__* start_c_index){
    num_type coef[kUnrollCoefficient];

    for(size_t k = 0; k < kBlockSize / kUnrollCoefficient; k+=kUnrollCoefficient){
        coef[0] = a_coeff * start_b_index[k];
        coef[1] = a_coeff * start_b_index[k + 1];
        coef[2] = a_coeff * start_b_index[k + 2];
        coef[3] = a_coeff * start_b_index[k + 3];

        start_c_index[k] += coef[0];
        start_c_index[k + 1] += coef[1];
        start_c_index[k + 2] += coef[2];
        start_c_index[k + 3] += coef[3];
    }
}

void matrix_product_row_block_unrolling_par(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count) {
    check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
    #ifdef RISCV_ARCH
        // Получаем сырые указатели с restrict для гарантии отсутствия алиасинга
        const double* __restrict a_ptr = a.data();
        const double* __restrict b_ptr = b.data();
        double* __restrict c_ptr = c.data();

        #pragma omp parallel for shared(a_ptr, b_ptr, c_ptr, a_row_count, a_column_count, b_column_count, kBlockSize)
        for(size_t ik = 0; ik < a_row_count; ik += kBlockSize){
            for(size_t jk = 0; jk < a_column_count; jk += kBlockSize){
                for(size_t kk = 0; kk < b_column_count; kk += kBlockSize){
                    for(size_t i = 0; i < kBlockSize; ++i){
                        double* c_row = c_ptr + (ik + i) * b_column_count + kk;
                        const double* a_row = a_ptr + (ik + i) * a_column_count + jk;

                        for (size_t j = 0; j < kBlockSize; ++j) {
                            double a_coeff = a_row[j];
                            const double* b_row = b_ptr + (jk + j) * b_column_count + kk;
                            double* c_block = c_row;

                            size_t k = 0;
                            // Векторная обработка блока
                            while (k < kBlockSize) {
                                // Устанавливаем активную длину для типа double с LMUL=2 (8 элементов за раз)
                                size_t vl = __riscv_vsetvl_e64m2(kBlockSize - k);

                                // Загружаем вектор из B
                                vfloat64m2_t v_b = __riscv_vle64_v_f64m2(b_row + k, vl);
                                // Загружаем текущий вектор из C
                                vfloat64m2_t v_c = __riscv_vle64_v_f64m2(c_block + k, vl);

                                // v_c = v_c + a_coeff * v_b  (FMA)
                                vfloat64m2_t v_res = __riscv_vfmacc_vf_f64m2(v_c, a_coeff, v_b, vl);

                                // Сохраняем результат обратно в C
                                __riscv_vse64_v_f64m2(c_block + k, v_res, vl);

                                k += vl;
                            }
                        }
                    }
                }
            }
        }
    #else
        size_t ik, jk, kk, i, j, k;
        num_type coef[kUnrollCoefficient];
        #pragma omp parallel for shared(a, b, c, a_row_count, a_column_count, b_column_count, kBlockSize) private(ik, jk, kk, i, j, k, coef)
        for(ik = 0; ik < a_row_count; ik += kBlockSize){
            for(jk = 0; jk < a_column_count; jk += kBlockSize){
                for(kk = 0; kk < b_column_count; kk += kBlockSize){
                    for(i = 0; i < kBlockSize; ++i){
                        num_type* start_c_index = c.data() + (ik + i) * b_column_count + kk;
                        for(j = 0; j < kBlockSize; ++j){
                            num_type a_coeff = a[(ik + i) * a_column_count + (jk + j)];
                            const num_type* start_b_index = b.data() + (jk + j) * b_column_count + kk;
                            for(k = 0; k < kBlockSize / kUnrollCoefficient; k += kUnrollCoefficient){
                                coef[0] = a_coeff * start_b_index[k];
                                coef[1] = a_coeff * start_b_index[k + 1];
                                coef[2] = a_coeff * start_b_index[k + 2];
                                coef[3] = a_coeff * start_b_index[k + 3];

                                start_c_index[k] += coef[0];
                                start_c_index[k + 1] += coef[1];
                                start_c_index[k + 2] += coef[2];
                                start_c_index[k + 3] += coef[3];
                            }
                        }
                    }
                }
            }
        }
    #endif
}

// bad on board
// void matrix_product_row_block_scalar_par(const vector<num_type>& a, const vector<num_type>& b, vector<num_type>& c, size_t a_row_count, size_t a_column_count, size_t b_column_count){
//     check_length(a.size(), b.size(), c.size(), a_row_count, a_column_count, b_column_count);
//     size_t ik, jk, kk, i, j;
//     #pragma omp parallel for shared(a, b, c, a_row_count, a_column_count, b_column_count, kBlockSize) private(ik, jk, kk, i, j)
//     for(ik = 0; ik < a_row_count; ik += kBlockSize){
//         for(jk = 0; jk < a_column_count; jk += kBlockSize){
//             for(kk = 0; kk < b_column_count; kk += kBlockSize){
//                 for(i = 0; i < kBlockSize; ++i){
//                     for(j = 0; j < kBlockSize; ++j){
//                         for(size_t i=0;i<kBlockSize;i+=kUnrollCoefficient){
//                             c[(ik + i) * b_column_count + (kk) + i] += b[(jk + j) * b_column_count + (kk) + i] * a[(ik + i) * a_column_count + (jk + j)];
//                             c[(ik + i) * b_column_count + (kk) + i + 1] += b[(jk + j) * b_column_count + (kk) + i + 1] * a[(ik + i) * a_column_count + (jk + j)];
//                             c[(ik + i) * b_column_count + (kk) + i + 2] += b[(jk + j) * b_column_count + (kk) + i + 2] * a[(ik + i) * a_column_count + (jk + j)];
//                             c[(ik + i) * b_column_count + (kk) + i + 3] += b[(jk + j) * b_column_count + (kk) + i + 3] * a[(ik + i) * a_column_count + (jk + j)];
//                         }
//                         // matrix_multiply_vector_by_number_optimal(b.data() + (jk + j) * b_column_count + (kk), c.data() + (ik + i) * b_column_count + (kk), a[(ik + i) * a_column_count + (jk + j)], kBlockSize);
//                         // for(k = 0; k < kBlockSize; ++k){
//                         //     c[(ik + i) * b_column_count + (kk + k)] += a[(ik + i) * a_column_count + (jk + j)] * b[(jk + j) * b_column_count + (kk + k)];
//                         // }
//                     }
//                 }
//             }
//         }
//     }
// }
