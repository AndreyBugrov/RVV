#pragma once

#include <map>  // std::map
#include <utility>  // std::pair
#include <string>  // report

#include "../tasks/base_task_runner.hpp"  // inheritance
#include "../tasks/base_task.hpp"  // base task creation
#include "experiment_task.hpp"
 
class ExperimentRunner: public BaseTaskRunner{
    protected:
    std::string delimiter_;
    std::map<std::string, VectorTask>* vector_tasks_;
    std::map<std::string, MatrixTask>* matrix_tasks_;
    std::map<std::string, GramSchmidtTask>* gram_schmidt_tasks_;
    std::map<std::string, QRTask>* qr_tasks_;
    public:
    ExperimentRunner(std::string delimiter): BaseTaskRunner(), delimiter_(delimiter){task_num_= 0;}
};