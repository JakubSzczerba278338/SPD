#include <vector>
#include <iostream>
#include <algorithm>
using Task = std::vector<int>;
using TaskVector = std::vector<Task>;

auto comp =  [](Task a, Task b){ return a[0] < b[0];};

int Calculate_Cmax(const TaskVector& Sol,size_t processors) {
    size_t N = Sol.size();
    std::vector<int> CompletionTimes(N, 0);
    CompletionTimes[0] = Sol[0][0];
    int start_time = 0;

    for(size_t proc = 0; proc < processors;++proc) {
        for(size_t i = 0; i < N; ++i) {
            if(proc == 0){ 
                if(i > 0) CompletionTimes[i] = CompletionTimes[i-1] + Sol[i][proc];
            }
            else {
                start_time = i == 0 ? CompletionTimes[i] : std::max(CompletionTimes[i-1],CompletionTimes[i]);
                CompletionTimes[i] = start_time + Sol[i][proc];
            }        
        }
    }
    return CompletionTimes[N-1];
}

TaskVector Complete_Search(TaskVector tasks, size_t processors){
    std::sort(tasks.begin(),tasks.end(),comp);
    TaskVector solution = tasks;
    int minCmax = Calculate_Cmax(tasks, processors);
    while(std::next_permutation(tasks.begin(), tasks.end(), comp)) {
        if(int currentCmax = Calculate_Cmax(tasks, processors); currentCmax < minCmax) {
            minCmax = currentCmax;
            solution = tasks;
        }
    }
    return solution;
}
// TaskVector solve(std::vector<int> Tasks) {
    
// }

int main() {
    Task task1 = {5,1,2};
    Task task2 = {2,2,5};
    Task task3 = {3,4,3};
    Task task4 = {5,4,4}; 
    Task task5 = {6,2,1};
    TaskVector sol = {task2,task1,task4,task3,task5};
    std::cout << Calculate_Cmax(sol,3)<<std::endl;

}





