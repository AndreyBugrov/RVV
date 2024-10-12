#include "experiment_runner.hpp"

std::map<std::string, int> ExperimentRunner::task_types_ = {
    std::pair<std::string, int>("vec", 0),
    std::pair<std::string, int>("matr", 1),
    std::pair<std::string, int>("g_sch", 2),
    std::pair<std::string, int>("qr", 3),
};

ExperimentRunner::ExperimentRunner(double min_value, double max_value,
size_t row_num, size_t column_num, size_t column_num2,
std::map<std::string, VectorTask>* vector_tasks = nullptr,
std::map<std::string, MatrixTask>* matrix_tasks = nullptr,
std::map<std::string, GramSchmidtTask>* gram_schmidt_tasks = nullptr,
std::map<std::string, QRTask>* qr_tasks = nullptr): BaseTaskRunner(), 
row_num_(row_num), column_num_(column_num), column_num2_(column_num2), min_value_(min_value), max_value_(max_value_){
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

void ExperimentRunner::print_header(std::ostream& stream = std::cout) const{
    stream<<"Function name  Time"<<"\n";
}

void ExperimentRunner::run_all(std::ostream& stream) const{
    print_header(stream);
    for(auto task: (*vector_tasks_)){
        run_by_name_unsafe(task.first, 0, stream);
    }
    for(auto task: (*matrix_tasks_)){
        run_by_name_unsafe(task.first, 1, stream);
    }
    for(auto task: (*gram_schmidt_tasks_)){
        run_by_name_unsafe(task.first, 2, stream);
    }
    for(auto task: (*qr_tasks_)){
        run_by_name_unsafe(task.first, 3, stream);
    }
}

void ExperimentRunner::run_by_name(const std::string& name, std::ostream& stream = std::cout) const{
    size_t underscore_position = name.find('_');
    if(underscore_position == std::string::npos){
        throw Exception(ErrorType::kValueError, generate_string("No underscore in experiment task name \"", name, "\""));
    }
    std::string experiment_function_type = name.substr(0, underscore_position);
    int task_type;
    try{
        task_type = task_types_.at(experiment_function_type);
    }catch(std::out_of_range ex){
        throw Exception(ErrorType::kValueError, generate_string("Unsupported experiment function type: ", experiment_function_type));
    }
    if(task_type<0 || task_type>3){
        throw Exception(ErrorType::kUnknownError, generate_string("Unknown task type number: ", task_type));
    }
    run_by_name_unsafe(name, task_type, stream);
}

void ExperimentRunner::run_by_name_unsafe(const std::string& name, int task_type, std::ostream& stream = std::cout) const{
    BaseTaskOutput task_output;
    switch (task_type)
    {
    case 0:
        {
            std::vector<double> a(row_num_);
            std::vector<double> b(row_num_);
            generate_rand_array(a.data(), row_num_, min_value_, max_value_);
            generate_rand_array(b.data(), row_num_, min_value_, max_value_);
            task_output = vector_tasks_->at(name).run(VectorProdInput(a, b, row_num_));
            break;
        }
    case 1:
        {
            std::vector<double> a(row_num_*column_num_);
            std::vector<double> b(column_num_*column_num2_);
            std::vector<double> c(row_num_*column_num2_);
            generate_rand_array(a.data(), row_num_*column_num_, min_value_, max_value_);
            generate_rand_array(b.data(), column_num_*column_num2_, min_value_, max_value_);
            task_output = matrix_tasks_->at(name).run(MatrixProdInput(a, b, c, row_num_, column_num_, column_num2_));
            break;
        }
        
        
    case 2:
        task_output = gram_schmidt_tasks_->at(name).run();
        break;
    case 3:
        task_output = qr_tasks_->at(name).run();
        break;
    }
    const size_t function_name_limit = 13;
    if(task_output.error_message() != ""){
        std::cerr<<"Error type: "<<task_output.what()<<"\n";
        std::cerr<<"Error message: "<<task_output.error_message()<<"\n";
    }
    else{
        stream<<name<<std::setw(function_name_limit-name.size()+1)<<task_output.time()<<"\n";
    }
}
