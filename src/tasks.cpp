#include <vector>
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
};

std::ifstream &operator>>(std::ifstream &ifstr,Task &task){
    int release_date,processing_time,due_date;
    ifstr >> release_date >> processing_time >> due_date;
    task.set_parameters(release_date,processing_time,due_date); 
    return ifstr;
}

std::vector<Task> Read_file(std::string file){
    int num_tasks = 0;
    std::ifstream File(file);
    File >> num_tasks;
    Task task;
    while (File >> task){

    }
}

int Lmax(std::vector<Task> permutation){
    int max_L=0;
    int L =0;
    int time=0;
    for(Task &task: permutation){
        time = task.calc_compl_time(time);
        L = task.calc_lateness(time);
        if(max_L < L){max_L = L;}
    }
    return max_L;
}

int main(){
    
}