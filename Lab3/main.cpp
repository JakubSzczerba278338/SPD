#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include <numeric>


using TaskVector = std::vector<int>;
using Solution = std::vector<std::pair<int,int>>;

std::ostream& operator <<(std::ostream& ostrm, const std::vector<int>& v){
    for (int i = 0; i <size(v) - 1; i++){
        ostrm<<v[i]<<", ";
    }
    ostrm<<v[size(v) - 1];
    return ostrm;
}

std::ostream& operator <<(std::ostream& ostrm, const std::vector<std::pair<int, int>>& v){
    for (int i = 0; i <size(v) - 1; i++){
        ostrm<<"("<<v[i].first<<", "<<v[i].second<<") ,";
    }
    ostrm<<"("<<v[size(v) - 1].first<<", "<<v[size(v) - 1].second<<")";
    return ostrm;
}

std::vector<int> operator /(std::vector<int> v,int k){
    for(int& element: v){
        element/=k;
    }
    return v;
}

void printMatrix(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return;
    for (const auto& row : matrix) {
        for (size_t j = 0; j < row.size() - 1; j++) {
            std::cout << row[j] << ", ";
        }
        std::cout << row[row.size() - 1] << std::endl;
    }
}


int FindMinRow(const std::vector<std::vector<int>>& matrix,int column){
    for(int i = 1;i<=matrix.size();i++){
        if(matrix[i][column] == 1) return i;
    }
    return -1;
}

Solution BinToSolution(TaskVector tasks,uint32_t bin){
    Solution sol;
    int N = tasks.size();
    sol.resize(N);

    for(int i =0; i < N; i++){
        sol[i] = {tasks[i], (bin >> (N - 1 - i)) & 1};
    }
    return sol;
}

// void NextSolution(Solution& sol){
//     uint32_t coded{};
//     int i = 0;
//     for (std::pair<int, int> & pair :sol){
//         if(pair.second){
//             coded |= 1<<i;
//         }
//         i++;
//     }
    
// }


int Cmax(Solution sol,int processors){
    std::vector<int> times{};
    times.resize(processors);

    for(int i = 0; i < sol.size(); ++i){
        times[sol[i].second] += sol[i].first;
    }   
    return *std::max_element(times.begin(),times.end());
}


int Cmax(TaskVector tasks,uint32_t sol){
    std::vector<int> times{};
    times.resize(2);
    int N = tasks.size();
    for(int i = 0; i < N; ++i){
        times[((sol >> i) & 1)] += tasks[N - 1 - i];
    }   
    return std::max(times[0],times[1]);
}

Solution LSA(TaskVector Tasks,int processors){
    Solution sol{};
    int N = size(Tasks);
    sol.resize(N);
    
    std::vector<int> times{};
    times.resize(processors);

    int idx_min {};

    for (int i=0; i < N ; i++){
        idx_min = std::distance(times.begin(),std::min_element(times.begin(),times.end()));
        times[idx_min] += Tasks[i];
        sol[i] = {Tasks[i],idx_min};
        
    }
    return sol;
}
Solution LPT(TaskVector Tasks,int processors){
    Solution sol{};
    int N = size(Tasks);
    sol.resize(N);
    
    std::vector<int> times{};
    times.resize(processors);

    int idx_min {};
    std::sort(Tasks.begin(),Tasks.end(),std::greater<int>());
    for (int i=0; i < N ; i++){
        idx_min = std::distance(times.begin(),std::min_element(times.begin(),times.end()));
        times[idx_min] += Tasks[i];
        sol[i] = {Tasks[i],idx_min};
    }
    return sol;

}

Solution PD(TaskVector Tasks) {
    int N = Tasks.size();
    int total_sum = 0;
    for (int element : Tasks) {
        total_sum += element;
    }
    
    int target = total_sum / 2;
    
    std::vector<std::vector<int>> matrix(N + 1, std::vector<int>(target + 1, 0));
    
    for (int i = 0; i <= N; i++) {
        matrix[i][0] = 1;
    }
    int sol_i{},sol_j{};
    for (int i = 1; i <= N; i++) {
        int value = Tasks[i - 1];
        for (int j = 1; j <= target; j++) {
            if (matrix[i - 1][j] == 1) {
                matrix[i][j] = 1;
            }
            if (j >= value && matrix[i-1][j-value] == 1){
                matrix[i][j] = 1;
                if(sol_j < j){
                    sol_i = i;
                    sol_j = j;
                }
            }
        }
    }

    
    
    Solution sol{};
    sol.resize(N);
    std::vector<int> forbidden_idx;
    sol[0] = {Tasks[sol_i - 1],0};
    forbidden_idx.push_back(sol_i - 1);
    int idx{1};
    int found{};
    sol_j = sol_j - sol[idx - 1].first;
    while(sol_j>0){
        found = FindMinRow(matrix, sol_j) - 1;
        sol[idx] = {Tasks[found],0};
        forbidden_idx.push_back(found);
        idx++;
        sol_j = sol_j - sol[idx - 1].first;
    }
    for(int i = 0; i < N; i++){
        if(count(forbidden_idx.begin(), forbidden_idx.end(), i) == 0){
           sol[idx] = {Tasks[i],1};
           idx++;
        } 
    }
    return sol;
}


Solution PD(TaskVector Tasks,TaskVector Tasks_k) {
    int N = Tasks_k.size();
    int total_sum = 0;
    for (int element : Tasks_k) {
        total_sum += element;
    }
    
    int target = total_sum / 2;
    
    std::vector<std::vector<int>> matrix(N + 1, std::vector<int>(target + 1, 0));
    
    for (int i = 0; i <= N; i++) {
        matrix[i][0] = 1;
    }
    int sol_i{},sol_j{};
    for (int i = 1; i <= N; i++) {
        int value = Tasks_k[i - 1];
        for (int j = 1; j <= target; j++) {
            if (matrix[i - 1][j] == 1) {
                matrix[i][j] = 1;
            }
            if (j >= value && matrix[i-1][j-value] == 1){
                matrix[i][j] = 1;
                if(sol_j < j){
                    sol_i = i;
                    sol_j = j;
                }
            }
        }
    }
    
    Solution sol_k{},sol{};
    sol.resize(N);
    sol_k.resize(N);
    std::vector<int> forbidden_idx;
    sol_k[0] = {Tasks_k[sol_i - 1],0};
    sol[0] = {Tasks[sol_i - 1],0};
    forbidden_idx.push_back(sol_i - 1);
    int idx{1};
    int found{};
    sol_j = sol_j - sol_k[idx - 1].first;
    while(sol_j>0){
        found = FindMinRow(matrix, sol_j) - 1;
        sol_k[idx] = {Tasks_k[found],0};
        sol[idx] = {Tasks[found],0};
        forbidden_idx.push_back(found);
        idx++;
        sol_j = sol_j - sol_k[idx - 1].first;
    }
    for(int i = 0; i < N; i++){
        if(count(forbidden_idx.begin(), forbidden_idx.end(), i) == 0){
           sol[idx] = {Tasks[i],1};
           idx++;
        } 
    }
    return sol;
}

Solution CS(TaskVector tasks){
    uint32_t coded{};
    int N = tasks.size();
    int tempCmax{};
    int minCmax = std::accumulate(tasks.begin(),tasks.end(),0);
    uint32_t best_coded{};
    while(coded <= std::pow(2,N-1) - 1){
        if( tempCmax = Cmax(tasks,coded) ; tempCmax < minCmax){
            minCmax = tempCmax;
            best_coded = coded;
        }
        coded++;
    }
    return BinToSolution(tasks, best_coded);
}

Solution PTAS(TaskVector tasks,int k){
    std::sort(tasks.begin(),tasks.end(),std::greater<int>());
    int N = tasks.size();
    Solution sol;
    sol.resize(N);
    TaskVector subtasks;
    subtasks.resize(N);
    
    for(int i=0;i<k;i++){
        subtasks[i] = tasks[i];
    }
    Solution subsolution = CS(subtasks);
    for(int i = 0; i < k;i++){
        sol[i] = subsolution[i];
    }

    for(int i = k; i<N;i++){
        subtasks[i - k] = tasks[i];
    }
    subsolution = LSA(subtasks,2);
    for(int i = k; i < N;i++){
        sol[i] = subsolution[i-k];
    }
    return sol;
}


Solution FPTAS(TaskVector tasks, int k){
    return PD(tasks,tasks/k);
}


int main(){
    TaskVector V{2, 5,  3, 5, 8};
    int processors = 2;

    std::cout<< "SUM: " <<std::accumulate(V.begin(),V.end(),0) << std::endl;
    Solution sol;
    sol =  PD(V);
    std::cout<<"PD -- Cmax: "<<Cmax(sol, processors)<<": "<<sol<<std::endl;
    sol =  LSA(V,processors);
    std::cout<<"LSA -- Cmax: "<<Cmax(sol, processors)<<": "<<sol<<std::endl;
    sol =  LPT(V,processors);
    std::cout<<"LPT -- Cmax: "<<Cmax(sol, processors)<<": "<<sol<<std::endl;
    sol = CS(V);
    std::cout<<"CS -- Cmax: "<<Cmax(sol, processors)<<": "<<sol<<std::endl;
    sol = PTAS(V,3);
    std::cout<<"PTAS -- Cmax: "<<Cmax(sol, processors)<<": "<<sol<<std::endl;
    sol = FPTAS(V,2);
    std::cout<<"FPTAS -- Cmax: "<<Cmax(sol, processors)<<": "<<sol<<std::endl;
}