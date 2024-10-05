#pragma once

#include <vector>

#include "../tasks/base_task.hpp"

struct VectorProdInput{
    const double* a;
    const double* b;
    size_t length;

    VectorProdInput(const double* vec_1, const double* vec_2, size_t length_input): a(vec_1), b(vec_2), length(length_input){}
    ~VectorProdInput() = default;
};

struct MatrixProdInput{
    const double* a;
    const double* b;
    double* c;
    size_t a_row_num;
    size_t a_column_num;
    size_t b_column_num;

    MatrixProdInput(const double* matr_1, const double* matr_2, double* result_matr, size_t a_row_num_input, size_t a_column_num_input, size_t b_column_num_input): 
    a(matr_1), b(matr_2), c(result_matr), a_row_num(a_row_num_input), a_column_num(a_column_num_input), b_column_num(b_column_num_input){}
    ~MatrixProdInput() = default;
};

struct GramSchmidtInput{
    const double* base_matrix;
    double* result_matrix;
    size_t rank;
    size_t column_length;

    GramSchmidtInput(const double* base_matr, double* result_matr, size_t rank_input, size_t column_length_input): 
    base_matrix(base_matr), result_matrix(result_matr), rank(rank_input), column_length(column_length_input){}
    ~GramSchmidtInput() = default;
};

struct QRInput{
    const double* base_matrix;
    double* Q_matrix;
    double* R_matrix;
    size_t base_matrix_row_num;
    size_t base_matrix_column_num;

    QRInput(const double* base_matr, double* Q_matrix_input, double* R_matrix_input, size_t row_num, size_t column_num): 
    base_matrix(base_matr), Q_matrix(Q_matrix_input), R_matrix(R_matrix_input), base_matrix_row_num(row_num), base_matrix_column_num(column_num)
    {}
    ~QRInput() = default;
};

class VectorTask: BaseTask<VectorProdInput, double, BaseTaskOutput>{
public:
    VectorTask(std::string name, std::function<double(VectorProdInput)> task=dumb_task<VectorProdInput, double>): BaseTask(name, task){}
};

class MatrixTask: BaseTask<MatrixProdInput, void, BaseTaskOutput>{
public:
    MatrixTask(std::string name, std::function<void(MatrixProdInput)> task=dumb_task<MatrixProdInput, void>): BaseTask(name, task){}
};

class GramSchmidtTask: BaseTask<GramSchmidtInput, void, BaseTaskOutput>{
public:
    GramSchmidtTask(std::string name, std::function<void(GramSchmidtInput)> task=dumb_task<GramSchmidtInput, void>): BaseTask(name, task){}
};

class QRTask: BaseTask<QRInput, void, BaseTaskOutput>{
public:
    QRTask(std::string name, std::function<void(QRInput)> task=dumb_task<QRInput, void>): BaseTask(name, task){}
};
