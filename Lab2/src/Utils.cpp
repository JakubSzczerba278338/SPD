#include "Utils.h"



int ReadFile(std::string& file,TaskVector& Tasks){
    std::ifstream File(file);
    int inst_num{},task_num{},proc_num{};
    File >> inst_num >> task_num >> proc_num;

    std::string dummy;
    std::getline(File,dummy);
    Tasks.reserve(task_num);
    Task task{1};
    while(task.getId() <= task_num && File >> task ){
        Tasks.push_back(task);
        task.changeId(task.getId() + 1);
    }
    return proc_num;
}