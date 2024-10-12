#pragma once

#include "../tasks/base_task.hpp"  // inheritance
#include "../algorithms/scalar_product.hpp"  // VectorProdInput
#include "../algorithms/matrix_product.hpp"  // MatrixProdInput
#include "../algorithms/gram_schmidt.hpp"  // GramSchmidtInput
#include "../algorithms/qr_decomposition.hpp"  // QRInput

template<typename FunctionInput, typename FunctionOutput>
class ExperimentTask: public BaseTask<FunctionInput, FunctionOutput, BaseTaskOutput>{
protected:
    std::function<FunctionOutput(FunctionInput)> open_blas_task_;
    std::function<FunctionOutput(FunctionInput)> boost_task_;
    std::function<FunctionOutput(FunctionInput)> eigen_task_;

public:
    ExperimentTask(std::string name, std::function<FunctionOutput(FunctionInput)> task=dumb_task,
    std::function<FunctionOutput(FunctionInput)> open_blas_task=dumb_task,
    std::function<FunctionOutput(FunctionInput)> boost_task=dumb_task,
    std::function<FunctionOutput(FunctionInput)> eigen_task=dumb_task): 
    BaseTask<ExperimentFunctionInput, ExperimentFunctionOutput, BaseTaskOutput>(name, task),
    open_blas_task_(open_blas_task), boost_task_(boost_task), eigen_task_(eigen_task){}
};

class VectorTask: ExperimentTask<VectorProdInput, double>{
public:
    VectorTask(std::string name, std::function<double(VectorProdInput)> task=dumb_task<VectorProdInput, double>,
    std::function<double(VectorProdInput)> open_blas_task=dumb_task<VectorProdInput, double>,
    std::function<double(VectorProdInput)> boost_task=dumb_task<VectorProdInput, double>,
    std::function<double(VectorProdInput)> eigen_task=dumb_task<VectorProdInput, double>): 
    ExperimentTask<VectorProdInput, double>(name, task, open_blas_task, boost_task, eigen_task){}
};

class MatrixTask: ExperimentTask<MatrixProdInput, void>{
public:
    MatrixTask(std::string name, std::function<void(MatrixProdInput)> task=dumb_task<MatrixProdInput, void>,
    std::function<void(MatrixProdInput)> open_blas_task=dumb_task<MatrixProdInput, void>,
    std::function<void(MatrixProdInput)> boost_task=dumb_task<MatrixProdInput, void>,
    std::function<void(MatrixProdInput)> eigen_task=dumb_task<MatrixProdInput, void>):
    ExperimentTask<MatrixProdInput, void>(name, task, open_blas_task, boost_task, eigen_task){}
};

class GramSchmidtTask: ExperimentTask<GramSchmidtInput, void>{
public:
    GramSchmidtTask(std::string name, std::function<void(GramSchmidtInput)> task=dumb_task<GramSchmidtInput, void>,
    std::function<void(GramSchmidtInput)> open_blas_task=dumb_task<GramSchmidtInput, void>,
    std::function<void(GramSchmidtInput)> boost_task=dumb_task<GramSchmidtInput, void>,
    std::function<void(GramSchmidtInput)> eigen_task=dumb_task<GramSchmidtInput, void>): 
    ExperimentTask<GramSchmidtInput, void>(name, task, open_blas_task, boost_task, eigen_task){}
};

class QRTask: ExperimentTask<QRInput, void>{
public:
    QRTask(std::string name, std::function<void(QRInput)> task=dumb_task<QRInput, void>,
    std::function<void(QRInput)> open_blas_task=dumb_task<QRInput, void>,
    std::function<void(QRInput)> boost_task=dumb_task<QRInput, void>,
    std::function<void(QRInput)> eigen_task=dumb_task<QRInput, void>):
    ExperimentTask<QRInput, void>(name, task, open_blas_task, boost_task, eigen_task){}
};
