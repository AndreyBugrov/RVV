#pragma once

#include <map>  // std::map
#include <utility>  // std::pair
#include <string>  // report
#include <stdexcept> // std::out_of_range from std::map::at()

#include "../tasks/base_task_runner.hpp"  // inheritance
#include "../tasks/base_task.hpp"  // base task creation
#include "experiment_task.hpp"  // experiment task classes
#include "../common/generators.hpp"  // generate_string

class ExperimentRunner: public BaseTaskRunner{
    protected:
    size_t vector_tasks_num_;
    size_t matrix_tasks_num_;
    size_t gram_schmidt_tasks_num_;
    size_t qr_tasks_num_;

    std::string header;
    
    std::map<std::string, VectorTask>* vector_tasks_;
    std::map<std::string, MatrixTask>* matrix_tasks_;
    std::map<std::string, GramSchmidtTask>* gram_schmidt_tasks_;
    std::map<std::string, QRTask>* qr_tasks_;

    static std::map<std::string, int> task_types_;
    public:
    ExperimentRunner(std::map<std::string, VectorTask>* vector_tasks = nullptr,
    std::map<std::string, MatrixTask>* matrix_tasks = nullptr,
    std::map<std::string, GramSchmidtTask>* gram_schmidt_tasks = nullptr,
    std::map<std::string, QRTask>* qr_tasks = nullptr);//: BaseTaskRunner(),
    // vector_tasks_(vector_tasks), matrix_tasks_(matrix_tasks), gram_schmidt_tasks_(gram_schmidt_tasks), qr_tasks_(qr_tasks)
    // {   
    //     task_num_= 0;
    //     vector_tasks_num_ = 0;
    //     matrix_tasks_num_ = 0;
    //     gram_schmidt_tasks_num_ = 0;
    //     qr_tasks_num_ = 0;
    //     if(vector_tasks_ != nullptr){
    //         vector_tasks_num_ = vector_tasks->size();
    //         task_num_ += vector_tasks_num_;
    //     }
    //     if(matrix_tasks_ != nullptr){
    //         vector_tasks_num_ = matrix_tasks_->size();
    //         task_num_ += vector_tasks_num_;
    //     }
    //     if(gram_schmidt_tasks_ != nullptr){
    //         gram_schmidt_tasks_num_ = gram_schmidt_tasks_->size();
    //         task_num_ += gram_schmidt_tasks_num_;
    //     }
    //     if(qr_tasks_ != nullptr){
    //         qr_tasks_num_ = qr_tasks_->size();
    //         task_num_ += qr_tasks_num_;
    //     }
    // }
    void run_all(std::ostream& stream = std::cout) override;
    void run_by_name(const std::string& name, std::ostream& stream = std::cout) override;
    ~ExperimentRunner() = default;
};