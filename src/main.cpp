#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>
#include "../inc/Task.hh"
#include <limits>
#include <sstream>
#include <utility>

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

/*
    Funkcja używana w algorytmie konstrukcyjnym do szukania najlepszego miejsca do wstawienia nowego Task'a.
*/
std::vector<Task> complete_search_insert(std::vector<Task> &tasks,Task &task) {
    tasks.reserve(tasks.size()+1); //Po zrobieniu insert chyba się pamięć przenosi w inne miejsce i iterator już wskazuje na gówno stąd alokujemy miejsce na jeden dodatkowy element 
    std::vector<Task> best_permutation = tasks;
    int min_max_L = std::numeric_limits<int>::max();
    auto end_1 = tasks.end();
    std::advance(end_1,1);
    for(auto it = tasks.begin() ; it != end_1 ; std::advance(it,1)){//
        tasks.insert(it,task);
        if (int currentLmax = Lmax(tasks); currentLmax < min_max_L) {
            min_max_L = currentLmax;
            best_permutation = tasks;
            
            // Do podglądania
            //std::cout << min_max_L << std::endl; 
            //std::cout << best_permutation << std::endl;
        }
        tasks.erase(it);
    }
    return best_permutation;
}

/*
    Ten algorytm działa tak: dodajemy kolejny task z tasków posortowanych po rj i wstawiamy go w miejsce, które minimalizuje funkcję Lmax. 
*/
std::vector<Task> construction_alg(std::vector<Task> &tasks){
    std::sort(tasks.begin(),tasks.end());
    std::vector<Task> current_perm;
    current_perm.reserve(tasks.size());
    for(auto& task: tasks){
        current_perm = complete_search_insert(current_perm,task);
    }
    return current_perm;
}

std::vector<Task>::iterator find_min_due_date(std::vector<Task> &tasks) { // co jeśli są równe?
    auto min_dd_it = tasks.begin();
    for(auto it = tasks.begin(); it != tasks.end(); it = std::next(it)) {
        if(int current_dd = it->get_due_date(); min_dd_it->get_due_date() > current_dd) {
            min_dd_it = it;
        }
    }
    return min_dd_it;
}

std::vector<Task> schrage(std::vector<Task> &tasks) {
    std::vector<Task> N = tasks;
    std::vector<Task> G;
    std::vector<Task> result;

    std::sort(N.begin(), N.end());
    int t = N[0].get_release_date();
    auto it = N.begin();

    while(!G.empty() || it != N.end()) {
        while(it->get_release_date() <= t && it != N.end()) {
            G.push_back(*it);
            it = std::next(it);
        }

        if(G.empty()) {
            t = it->get_release_date();
        } else {
            auto min_dd_it = find_min_due_date(G);
            if(min_dd_it != G.end()) {
                G.erase(min_dd_it);
                result.push_back(*min_dd_it);
                t += min_dd_it->get_processing_time();
            }
        }
    }
    return result;
}

std::vector<std::pair<Task,int>> schrage_preemptive(std::vector<Task> &tasks) {
    std::vector<Task> N = tasks;
    std::vector<Task> G;
    std::vector<std::pair<Task,int>> result;
    int time_processed;

    std::sort(N.begin(), N.end());
    int t = N[0].get_release_date();
    auto it = N.begin();

    while(!G.empty() || it != N.end()) {
        while(it->get_release_date() <= t && it != N.end()) {
            G.push_back(*it);
            it = std::next(it);
        }
        if(G.empty()) {
            t = it->get_release_date();
        } else {
            auto min_dd_it = find_min_due_date(G);
            if(min_dd_it != G.end()) {
                if(int temp = it->get_release_date(); temp < t + min_dd_it->get_processing_time() - min_dd_it->get_time_processed() && it != N.end()){
                    time_processed = temp - t;
                }
                else{
                    time_processed = min_dd_it->get_processing_time() - min_dd_it->get_time_processed();
                }
                result.push_back(std::pair<Task,int>(*min_dd_it,time_processed));
                min_dd_it->change_time_processed(time_processed);
                if(min_dd_it->is_finished()){
                    G.erase(min_dd_it);
                }
                t += time_processed;
            }
        }
    }
    return result;
}


int main(int argc, char* argv[]){
    if(argc<2){
        std::cout<<"Użycie: " << argv[0] <<" <ścieżka do pliku z taskami>"<<std::endl;
        return 1;
    }

    std::string file_path = argv[1];
    std::vector<Task> tasks = Read_file(file_path);
    std::cout << "Zbiór tasków:" << std::endl << tasks << std::endl << std::endl;
    // std::vector<Task> v = complete_search(tasks); 
    //std::vector<Task> v = construction_alg(tasks);
    //auto v = schrage(tasks);
    auto v = schrage_preemptive(tasks);
    std::cout << "Najlepsza permutacja to: " << std::endl << v << "Lmax = " <<std::endl; //Lmax(v) << std::endl;
}