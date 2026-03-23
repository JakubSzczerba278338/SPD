#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
class Task{
    int _release_date;
    int _processing_time;
    int _due_date; 
    public:
    Task(){};       
    int calc_compl_time(int start_time) {
        return start_time > _release_date ? start_time + _processing_time : _release_date + _processing_time;
    }
    int calc_lateness(int compl_time){
        return compl_time - _due_date;
    }
    bool operator<(Task &other){
        return _release_date < other._release_date;
    }
    void set_parameters(int release_date,int processing_time, int due_date){_release_date = release_date; 
                                                        _processing_time = processing_time; _due_date = due_date;}
    int get_release_date(){
        return _release_date;
    }
    int get_processing_time(){
        return _processing_time;
    }
    int get_due_date(){
        return _due_date;
    }
};
std::ostream &operator<<(std::ostream &ostr,Task &task){
    return ostr<<task.get_release_date()<<", "<<task.get_processing_time()<<", "<<task.get_due_date();
}

std::istream &operator>>(std::istream &istr,Task &task){
    int release_date,processing_time,due_date;
    istr >> release_date >> processing_time >> due_date;
    task.set_parameters(release_date,processing_time,due_date); 
    return istr;
}

std::vector<Task> Read_file(std::string file){
    int num_tasks = 0;
    std::ifstream File(file);
    File >> num_tasks;
    Task task;
    std::vector<Task> v_Tasks;
    v_Tasks.reserve(num_tasks);
    while (File >> task){
        v_Tasks.push_back(std::move(task));
    }
    return v_Tasks;
}

int Lmax(std::vector<Task> permutation){
    int max_L=0;
    int L =0;
    int time=0;
    for(auto&& task: permutation){
        time = task.calc_compl_time(time);
        L = task.calc_lateness(time);
        if(max_L < L){max_L = L;}
    }
    return max_L;
}

int main(){
    std::vector<Task> tasks = Read_file("taski.txt");
    for(auto&& task: tasks){
        std::cout<<task<<std::endl;
    }
    std::cout<<"Metryka = "<<Lmax(tasks)<<std::endl;
}