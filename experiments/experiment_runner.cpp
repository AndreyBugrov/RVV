#include "experiment_runner.hpp"

std::map<std::string, int> ExperimentRunner::task_types_ = {
    std::pair<std::string, int>("vec", 0),
    std::pair<std::string, int>("matr", 1),
    std::pair<std::string, int>("gram_shmidt", 2),
    std::pair<std::string, int>("qr", 3),
};

ExperimentRunner::ExperimentRunner(std::map<std::string, VectorTask>* vector_tasks = nullptr,
std::map<std::string, MatrixTask>* matrix_tasks = nullptr,
std::map<std::string, GramSchmidtTask>* gram_schmidt_tasks = nullptr,
std::map<std::string, QRTask>* qr_tasks = nullptr){
    BaseTaskRunner();
    vector_tasks_ = vector_tasks;
    matrix_tasks_ = matrix_tasks;
    gram_schmidt_tasks_ = gram_schmidt_tasks;
    qr_tasks_ = qr_tasks;
    task_num_= 0;
    vector_tasks_num_ = 0;
    matrix_tasks_num_ = 0;
    gram_schmidt_tasks_num_ = 0;
    qr_tasks_num_ = 0;
    if(vector_tasks_ != nullptr){
        vector_tasks_num_ = vector_tasks->size();
        task_num_ += vector_tasks_num_;
    }
    if(matrix_tasks_ != nullptr){
        vector_tasks_num_ = matrix_tasks_->size();
        task_num_ += vector_tasks_num_;
    }
    if(gram_schmidt_tasks_ != nullptr){
        gram_schmidt_tasks_num_ = gram_schmidt_tasks_->size();
        task_num_ += gram_schmidt_tasks_num_;
    }
    if(qr_tasks_ != nullptr){
        qr_tasks_num_ = qr_tasks_->size();
        task_num_ += qr_tasks_num_;
    }
}

void ExperimentRunner::run_all(std::ostream& stream){

}

void ExperimentRunner::run_by_name(const std::string& name, std::ostream& stream = std::cout){
    size_t underscore_position = name.find('_');
    if(underscore_position == std::string::npos){
        throw Exception(ErrorType::kValueError, generate_string("No underscore in experiment task name \"", name, "\""));
    }
    std::string experiment_function_type = name.substr(0, underscore_position);
    int task_type;
    try{
        task_type = task_types_.at(experiment_function_type);
    }catch(std::out_of_range ex){
        throw Exception(ErrorType::kValueError, generate_string("Unsupported experimental function type: ", experiment_function_type));
    }
    switch (task_type)
    {
    case 0:
        /* code */
        break;
    case 2:
        /* code */
        break;
    case 3:
        /* code */
        break;    
    default:
        throw Exception(ErrorType::kUnknownError, generate_string("Unknow task type number: ", task_type));
        break;
    }
}
