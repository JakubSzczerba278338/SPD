#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>


using TaskVector = std::vector<int>;
using Solution = std::vector<std::pair<int,int>>;

std::ostream& operator <<(std::ostream& ostrm, std::vector<int>& v){
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


int Cmax(Solution sol,int processors){
    std::vector<int> times{};
    times.resize(processors);

    for(int i = 0; i < sol.size(); ++i){
        times[sol[i].second] += sol[i].first;
    }   
    return *std::max_element(times.begin(),times.end());
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

Solution PD(TaskVector Tasks, int processors = 2) {
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

int main(){
    TaskVector V{2, 5, 7, 4, 2, 3, 5, 8,2, 1};
    int processors = 2;
    Solution sol = LPT(V,processors);
    // std::cout<<Cmax(sol, processors);
    std::cout << PD(V)<<std::endl;

}