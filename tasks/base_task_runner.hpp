#pragma once

#include <cstddef>  // size_t
#include <string>  // report
#include <iostream> // output format

class BaseTaskRunner{
protected:
    std::string report_;
    size_t task_num_;
public:
    BaseTaskRunner(): task_num_(0), report_(""){}
    BaseTaskRunner(BaseTaskRunner&& task_runner) = default;
    BaseTaskRunner(const BaseTaskRunner& task_runner) = delete; // to prevent copying pointer from inner scope
    virtual void run_all(std::ostream& stream) = 0;
    void resize(size_t new_task_num){task_num_ = new_task_num;}
    size_t task_num() const{return task_num_;}
    virtual ~BaseTaskRunner() = 0;
};