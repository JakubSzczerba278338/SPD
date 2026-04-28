#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>
#include "../inc/Task.hh"
#include <limits>
#include <sstream>
#include <chrono>
#include <queue>


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
    for(int i = 0; i < num_tasks; i++) {
        File >> task;
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
    Przeciążenie Lmax dla Schrage wywłaszczającego
*/
int Lmax(std::vector<std::pair<Task,int>> &permutation){
    int max_L = 0;
    int L = 0;
    int time = 0;
    for(auto& pair: permutation){
        time = pair.first.calc_compl_time(time,pair.second);
        L = pair.first.calc_lateness(time);
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

/*
    Implementacja algorytmu Schrage.
*/
std::vector<Task> schrage(std::vector<Task> &tasks) { // prio queue zamiast vector
    std::vector<Task> N = tasks;
    auto cmp = [](const Task &t1, const Task &t2) { 
        return t1.get_due_date() > t2.get_due_date(); // Uwaga: '>' dla kolejki min-priority
    };

    std::priority_queue<Task, std::vector<Task>, decltype(cmp)> G(cmp);
    std::vector<Task> result;

    std::sort(N.begin(), N.end()); // sortujemy po rj
    int t = N[0].get_release_date();
    auto it = N.begin();

    while(!G.empty() || it != N.end()) {
        while(it != N.end() && it->get_release_date() <= t) {
            G.push(*it);
            it = std::next(it);
        }
        
        if(G.empty()) {
            t = it->get_release_date();
        } else {
            const auto& min_dd = G.top();
            //std::cout<<*min_dd_it<<std::endl;
            result.push_back(min_dd);
            G.pop();
            t += min_dd.get_processing_time();
        }
        
    }
    return result;
}
/*
    Schrage z wywłaszczaniem, w skrócie działa tak, że wykonujemy zadanie do momentu gdy następne w posortowanym po release date zbiorze tasków 
    będzie dostępne, wtedy przerywamy wykonywanie, dodajemy nowego taska i znowu wybieramy o minimalnym due date ze zbioru gotowych
*/
std::vector<std::pair<Task,int>> schrage_preemptive(std::vector<Task> &tasks) {
    std::vector<Task> N = tasks;
    auto cmp = [](const Task &t1, const Task &t2) { 
        return t1.get_due_date() > t2.get_due_date(); // Uwaga: '>' dla kolejki min-priority
    };
    std::priority_queue<Task, std::vector<Task>, decltype(cmp)> G(cmp);
    std::vector<std::pair<Task,int>> result;
    int time_processed; //zmienna do reprezentacji rozwiążania

    std::sort(N.begin(), N.end());
    int t = N[0].get_release_date();
    auto it = N.begin();

    while(!G.empty() || it != N.end()) {
        while(it->get_release_date() <= t && it != N.end()) {
            G.push(*it);
            it = std::next(it);
        }
        if(G.empty()) {
            t = it->get_release_date();
        } else {
            auto& min_dd = G.top();
            
            //jeśli jest dostępne nowe zadanie zanim skończy się wykonywać bieżące to wykonuje się ono do momentu dostępności następnego
            if(int temp = it->get_release_date(); temp < t + min_dd.get_processing_time() - min_dd.get_time_processed() && it != N.end()){ 
                time_processed = temp - t;
            }
            else{
                time_processed = min_dd.get_processing_time() - min_dd.get_time_processed();
            }
            min_dd.change_time_processed(time_processed);
            result.push_back(std::pair<Task,int>(min_dd, time_processed));
            //jeśli zadanie wykonało się do końca usuń ze zbioru zadań gotowych
            if(min_dd.is_finished()){
                G.pop();
            }
            t += time_processed;
            
        }
    }
    return result;
}

template<typename Func>
void benchmark(const std::string& file_path, Func algorithm, int iter) {

std::chrono::duration<double> elapsed{};
auto tasks_start = Read_file(file_path);
auto v = algorithm(tasks_start);
for (int i = 0; i < iter; ++i) {
    auto tasks = Read_file(file_path);
    auto start = std::chrono::high_resolution_clock::now();
    algorithm(tasks);
    auto end = std::chrono::high_resolution_clock::now();
    elapsed += end - start;
}

std::cout << "Time: "<<(elapsed / iter).count() << "s"<< std::endl;
std::cout << "Lmax: " << Lmax(v) << std::endl;
//std::cout << "Best perm: " << std::endl << v << std::endl;
}

int main(int argc, char* argv[]){
    if(argc<2){
        std::cout<<"Użycie: " << argv[0] <<" <ścieżka do pliku z taskami>"<<std::endl;
        return 1;
    }

    int iter = 10;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        std::cout << "BENCHMARK - " << arg << std::endl;
        // std::cout << "============== complete search ==============" << std::endl;
        // benchmark(arg, complete_search, iter);
        std::cout << "============== construcion ==============" << std::endl;
        benchmark(arg, construction_alg, iter);
        std::cout << "============== schrage ==============" << std::endl;
        benchmark(arg, schrage, iter);
        std::cout << "============== schrage preempt ==============" << std::endl;
        benchmark(arg, schrage_preemptive, iter);
    }
}