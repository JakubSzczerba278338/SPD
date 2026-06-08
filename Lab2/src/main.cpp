#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>

#include "Algorithms.h"
#include "Utils.h"



using std::cout, std::endl;


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Użycie: " << argv[0] << " <ścieżka_do_pliku>" << std::endl;
        return 1;
    }
    std::string s{argv[1]};
    TaskVector tasks;
    int processors = ReadFile(s,tasks);
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
    TaskVector sol_NEH = NEH(tasks, processors);
    TaskVector sol_FNEH = FNEH(tasks, processors);
    TaskVector sol_Johnson = johnson(tasks);
    
    // printTaskOrder(solution);
    // cout << calculate_Cmax(solution, processors) << endl;

    printTaskOrder(sol_NEH);
    cout << calculate_Cmax(sol_NEH, processors) << endl;

    printTaskOrder(sol_FNEH);
    cout << calculate_Cmax(sol_FNEH, processors) << endl;

    printTaskOrder(sol_Johnson);
    cout << calculate_Cmax(sol_Johnson, processors) << endl;
}





