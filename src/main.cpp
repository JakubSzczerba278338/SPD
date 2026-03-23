#include <vector>
#include <iostream>
#include <istream>
#include <fstream>

#include "Task.hh"

/* 
czyta plik o podanej nazwie, którego format musi być:

<number_of_tasks>
<task_1_release_date> <task_1_processing_time> <task_1_due_date>
<task_2_release_date> <task_2_processing_time> <task_2_due_date>
...
<task_N_release_date> <task_N_processing_time> <task_N_due_date> 
EOF

A następnie wczytuje dane z pliku do obiektów klasy Task.
Zwraca wektor wczytanych Tasków.
*/
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

/*
    Funkcja przyjmuje wektor zadań, liczy opóźnienie dla każdego z zadań i zwraca największą wartość opóźnienia spośród wszystkich zadań.
*/
int Lmax(std::vector<Task> permutation){
    int max_L = 0;
    int L = 0;
    int time = 0;
    for(auto& task: permutation){
        time = task.calc_compl_time(time);
        L = task.calc_lateness(time);
        if(max_L < L){max_L = L;}
    }
    return max_L;
}

int main(){
    std::vector<Task> tasks = Read_file("taski.txt");
    for(auto& task: tasks){
        std::cout << task << std::endl;
    }
    std::cout << "Metryka = " << Lmax(tasks) << std::endl;
}