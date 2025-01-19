#pragma once

#include <chrono>  // time measurements
#include <string>  // report
#include <ctime>  // get current date and time
#include <iostream>  // output stream

#include "test_task.hpp"  // TestTask, TestOuput classes

void run_tests(const std::vector<TestTask>& test_tasks, size_t task_count, std::ostream& stream);