#include <vector>
#include <iostream>
#include <algorithm>

using Task = std::pair<int, std::vector<int>>;
using TaskVector = std::vector<Task>;

using std::cout, std::endl;

Task makeTask(int id, std::vector<int> vector) {
    return std::make_pair(id, vector);
}

auto taskComp =  [](Task a, Task b){ return a.second[0] < b.second[0];};

int calculate_Cmax(const TaskVector& tasks,size_t processors) {
    size_t N = tasks.size();
    std::vector<int> completionTimes(N, 0);
    completionTimes[0] = tasks[0].second[0];
    int start_time = 0;

    for(size_t proc = 0; proc < processors; ++proc) {
        for(size_t i = 0; i < N; ++i) {
            if(proc == 0){ 
                if(i > 0) completionTimes[i] = completionTimes[i-1] + tasks[i].second[proc];
            }
            else {
                start_time = i == 0 ? completionTimes[i] : std::max(completionTimes[i-1],completionTimes[i]);
                completionTimes[i] = start_time + tasks[i].second[proc];
            }        
        }
    }
    return completionTimes[N-1];
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

void printTaskOrder(const TaskVector& tasks) {
    for(const auto& task: tasks) {
        cout << task.first << " ";
    }
    cout << endl;
}

TaskVector NEH(TaskVector tasks, size_t processors) {
    std::sort(tasks.begin(), tasks.end(), taskComp);
    size_t N = tasks.size();
    int minCmax;
    TaskVector solution;
    solution.push_back(tasks[0]);

    TaskVector bestSolution;

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



int main() {
    Task task1 = makeTask(1, {5,1,2});
    Task task2 = makeTask(2, {2,2,5});
    Task task3 = makeTask(3, {3,4,3});
    Task task4 = makeTask(4, {5,4,4});
    Task task5 = makeTask(5, {6,2,1});

    TaskVector tasks = {task1, task2, task3, task4, task5};
    TaskVector solution = complete_search(tasks, 3);
    TaskVector sol_NEH = NEH(tasks, 3);
    
    printTaskOrder(solution);
    cout << calculate_Cmax(solution, 3) << endl;

    printTaskOrder(sol_NEH);
    cout << calculate_Cmax(sol_NEH, 3) << endl;
}





