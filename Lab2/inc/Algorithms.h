#pragma once

#include <vector>


#include "Task.h"

using TaskVector = std::vector<Task>;
using CompletionTimesTable = std::vector<std::vector<int>>;
using CTT = CompletionTimesTable;


bool taskComp(const Task& a, const Task& b);
void printTaskOrder(const TaskVector& tasks);
void printCTT(const CTT& ctt, int processors);
void printSolution(const TaskVector& tVector, int processors);
int calculate_Cmax(const TaskVector& tasks, size_t processors, CompletionTimesTable&& completionTimes = CTT(0,std::vector<int>(0,0)), int index = 0);
CTT construct_completionTimes(const TaskVector& tasks, size_t processors, CompletionTimesTable& completionTimes, int index = 0);
TaskVector complete_search(TaskVector tasks, size_t processors);
TaskVector NEH(TaskVector tasks, size_t processors);
TaskVector johnson(TaskVector tasks);

TaskVector FNEH(TaskVector tasks, size_t processors);