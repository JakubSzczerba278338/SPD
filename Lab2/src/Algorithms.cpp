#include <iostream>
#include <algorithm>
#include <iomanip>

#include "Algorithms.h"



using std::cout, std::endl;




bool taskComp(const Task& a, const Task& b) { return a.getJobsSum() < b.getJobsSum();}

void printTaskOrder(const TaskVector& tasks) {
    for(const auto& task: tasks) {
        cout << task.getId() << " ";
    }
    cout << endl;
}
void printCTT(const CTT& ctt, int processors) {
    size_t N = ctt.size();
    
    cout << "   | ";
    for(size_t pos = 0; pos < N; ++pos) {
        cout << std::setw(4) << pos + 1 << " | ";
    }
    cout << endl;

    cout << "---|-";
    for(size_t pos = 0; pos < N; ++pos) {
        cout << "-----|-";
    }
    cout << endl;

    for(size_t proc = 0; proc < processors; ++proc) {
        cout << proc + 1 << ". | ";
        for(size_t pos = 0; pos < N; ++pos) {
            cout << std::setw(4) << ctt[pos][proc] << " | ";
        }
        cout << endl;
    }
    cout << endl;
}
// int calculate_Cmax(const TaskVector& tasks, size_t processors) {
//     size_t N = tasks.size();
//     std::vector<int> completionTimes(N, 0);
//     completionTimes[0] = tasks[0].getJobs()[0];
//     int start_time = 0;

//     for(size_t proc = 0; proc < processors; ++proc) {
//         for(size_t i = 0; i < N; ++i) {
//             if(proc == 0){ 
//                 if(i > 0) completionTimes[i] = completionTimes[i-1] + tasks[i].getJobs()[proc];
//             }
//             else {
//                 start_time = i == 0 ? completionTimes[i] : std::max(completionTimes[i-1], completionTimes[i]);
//                 completionTimes[i] = start_time + tasks[i].getJobs()[proc];
//             }        
//         }
//     }
//     return completionTimes[N-1];
// }

int calculate_Cmax(const TaskVector& tasks, size_t processors, CompletionTimesTable&& completionTimes, int index) {
    size_t N = tasks.size();
    int start_time = 0;
    
    if(completionTimes.size() == 0){
        completionTimes = CTT(N,std::vector<int>(processors,0));
    }

    if(index == N - 1 || index == 0) { index = 0; completionTimes[0][0] = tasks[0].getJobs()[0];}
    for(size_t proc = 0; proc < processors; ++proc) {
        for(size_t pos = index; pos < N; ++pos) {
            if(proc == 0) { 
                if(pos > 0) completionTimes[pos][proc] = completionTimes[pos-1][proc] + tasks[pos].getJobs()[proc];
            }
            else {
                start_time = pos == 0 ? completionTimes[pos][proc-1] : std::max(completionTimes[pos-1][proc], completionTimes[pos][proc-1]);
                completionTimes[pos][proc] = start_time + tasks[pos].getJobs()[proc];
            }        
        }
    }
    // printCTT(completionTimes, processors);
    return completionTimes[N-1][processors-1];
}

TaskVector complete_search(TaskVector tasks, size_t processors){
    std::sort(tasks.begin(),tasks.end(), taskComp);
    int minCmax = calculate_Cmax(tasks, processors);
    TaskVector solution = tasks;
    

    while(std::next_permutation(tasks.begin(), tasks.end(), taskComp)) {
        if(int currentCmax = calculate_Cmax(tasks, processors); currentCmax < minCmax) {
            minCmax = currentCmax;
            solution = tasks;
        }
    }
    return solution;
}

TaskVector NEH(TaskVector tasks, size_t processors) {
    std::sort(tasks.begin(), tasks.end(), taskComp);
    size_t N = tasks.size();
    int minCmax;
    TaskVector solution;
    solution.reserve(N);
    solution.push_back(tasks[0]);

    TaskVector bestSolution;
    bestSolution.reserve(N);

    for(int i = 1; i < N; ++i) {
        minCmax = __INT_MAX__;
        Task currentTask = tasks[i];
        bestSolution = solution;

        for(int j = 0; j <= i; ++j) {
            TaskVector temp = solution;
            temp.insert(temp.begin() + j, currentTask);
            if(int currentCmax = calculate_Cmax(temp, processors); currentCmax < minCmax) {
                minCmax = currentCmax;
                bestSolution = temp;
            }
        }
        solution = bestSolution;
    }
    return solution;
}

TaskVector johnson(TaskVector tasks) {
    size_t N = tasks.size();
    TaskVector J_L, J_P;
    J_L.reserve(N);
    J_P.reserve(N);

    for(const auto& task: tasks) {
        task.getJobs()[0] < task.getJobs()[1] ? J_L.push_back(task) : J_P.push_back(task);
    }

    std::sort(J_L.begin(), J_L.end(), [](const Task& a, const Task& b){ return a.getJobs()[0] < b.getJobs()[0];});
    std::sort(J_P.begin(), J_P.end(), [](const Task& a, const Task& b){ return a.getJobs()[1] > b.getJobs()[1];});
    
    J_L.insert(J_L.end(), J_P.begin(), J_P.end());
    return J_L;
}

TaskVector FNEH(TaskVector tasks, size_t processors) {
    std::sort(tasks.begin(), tasks.end(), taskComp);
    size_t N = tasks.size();
    int minCmax;
    TaskVector solution;
    CTT completionTimes(N, std::vector<int>(processors, 0));

    solution.reserve(N);
    solution.push_back(tasks[0]);

    TaskVector bestSolution;
    bestSolution.reserve(N);

    for(int i = 1; i < N; ++i) {
        minCmax = __INT_MAX__;
        Task currentTask = tasks[i];
        bestSolution = solution;

        for(int j = i; j >= 0; --j) {
            TaskVector temp = solution;
            temp.insert(temp.begin() + j, currentTask);
            if(int currentCmax = calculate_Cmax(temp, processors, std::move(completionTimes), j); currentCmax < minCmax) {
                minCmax = currentCmax;
                bestSolution = temp;
            }
        }
        solution = bestSolution;
    }
    return solution;
}