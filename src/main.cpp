#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>
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
int Lmax(std::vector<Task> &permutation){
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
/*
    Implementacja Przeglądu Zupełnego - przyjmujemy referencję na wektor zadań i zwracamy permutację minimalizującą Lmax 
*/
std::vector<Task> complete_search(std::vector<Task> &tasks) {
    std::sort(tasks.begin(), tasks.end());
    std::vector<Task> best_permutation = tasks;
    int min_max_L = Lmax(tasks);
    while(std::next_permutation(tasks.begin(),tasks.end())){ //next_permutation zwraca False gdy nie ma następnej permutacji w porządku leksykograficznym.
        if (int currentLmax = Lmax(tasks); currentLmax < min_max_L) {
            min_max_L = currentLmax;
            best_permutation = tasks;
            
            // Do podglądania
            // std::cout << min_max_L << std::endl; 
            // std::cout << best_permutation << std::endl;
        }
    }
    return best_permutation;
}
int main(int argc, char* argv[]){
    if(argc<2){
        std::cout<<"Użycie: " << argv[0] <<" <ścieżka do pliku z taskami>"<<std::endl;
        return 1;
    }

    std::string file_path = argv[1];
    std::vector<Task> tasks = Read_file(file_path);
    std::cout << "Zbiór tasków:" << std::endl << tasks << std::endl << std::endl;
    std::vector<Task> v = complete_search(tasks); 
    std::cout << "Najlepsza permutacja to: " << std::endl << v << "Lmax = " << Lmax(v) << std::endl;
}