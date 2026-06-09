#include "Utils.h"



int ReadFile(std::ifstream& File,TaskVector& Tasks){
    int task_num{},proc_num{},task_id{1};
    File >> task_num >> proc_num;
    std::string dummy;
    std::getline(File,dummy);
    
    Task task{task_id };
    Tasks.resize(task_num);
    while(task_id <= task_num && File >> task ){
        Tasks[task_id-1] = task;
        task_id++;
        task.changeId(task_id );
    }
    
    return proc_num;
}