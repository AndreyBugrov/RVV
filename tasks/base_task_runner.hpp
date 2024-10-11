#pragma once

#include <cstddef>  // size_t
#include <string>  // report
#include <iostream> // output format

class BaseTaskRunner{
protected:
    size_t task_num_;
public:
    BaseTaskRunner(): task_num_(0){}
    BaseTaskRunner(BaseTaskRunner&& task_runner) = default;
    BaseTaskRunner(const BaseTaskRunner& task_runner) = delete; // to prevent copying pointer from inner scope
    void run_all(std::ostream& stream = std::cout){};
    void run_by_name(const std::string& name, std::ostream& stream = std::cout){};
    void resize(size_t new_task_num){task_num_ = new_task_num;}
    size_t task_num() const{return task_num_;}
    virtual ~BaseTaskRunner() = default;
};