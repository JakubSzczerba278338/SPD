#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <chrono>
#include "Algorithms.h"
#include "Utils.h"



using std::cout, std::endl;


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Użycie: " << argv[0] << " <ścieżka_do_pliku> " << std::endl;
        return 1;
    }
    std::ofstream results_file("wyniki_benchmarku.txt");
    if (!results_file.is_open()) {
        std::cerr << "Nie można otworzyć pliku do zapisu!" << std::endl;
        return 1;
    }
    std::string s{argv[1]};
    int inst_num{},CMAX{};
    std::chrono::duration<double> elapsed{};
    std::ifstream File{s};
    File >> inst_num;
    TaskVector tasks;
    while(inst_num > 0){
        int processors = ReadFile(File,tasks);
        inst_num--;

        results_file << (120 - inst_num) << ". " << tasks.size() << ", "<< processors <<std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        TaskVector sol = johnson(tasks);
        auto end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        CMAX = calculate_Cmax(sol, processors);
        results_file<< CMAX << ", "<< elapsed<<std::endl;
        results_file<<"____________________________________________"<<std::endl;
        
    }



    // Task task1(1, {5,1,2});
    // Task task2(2, {2,2,5});
    // Task task3(3, {3,4,3});
    // Task task4(4, {5,4,4});
    // Task task5(5, {6,2,1});
    // for (auto & task : tasks){
    //     std::cout<<"ID: "<<task.getId()<<std::endl;
    //     for(auto & job : task.getJobs()){
    //         std::cout<<job<<", ";
    //     }
    //     std::cout<<std::endl;
    // }
    // TaskVector tasks = {task2, task4, task3, task5, task1};

    // TaskVector solution = complete_search(tasks, processors);
    // printTaskOrder(solution);
    // cout << calculate_Cmax(solution, processors) << endl;

    
    
    // TaskVector sol_NEH = NEH(tasks, processors);
    // printTaskOrder(sol_NEH);
    // cout << calculate_Cmax(sol_NEH, processors) << endl;

    // TaskVector sol_FNEH = FNEH(tasks, processors);
    // printTaskOrder(sol_FNEH);
    // cout << calculate_Cmax(sol_FNEH, processors) << endl;
    
    // TaskVector sol_Johnson = johnson(tasks);
    // printTaskOrder(sol_Johnson);
    // cout << calculate_Cmax(sol_Johnson, processors) << endl;
    
    
}





