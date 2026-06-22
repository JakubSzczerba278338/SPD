#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

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



struct BenchResult {
    int cmax;
    double time_ms;
};

BenchResult benchmark(const std::string& name,
                      std::function<Solution(TaskVector)> algo,
                      const TaskVector& tasks,
                      int processors,
                      int opt_cmax = -1)
{
    auto t0 = std::chrono::high_resolution_clock::now();
    Solution sol = algo(tasks);
    auto t1 = std::chrono::high_resolution_clock::now();

    int cmax = Cmax(sol, processors);
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    double err = (opt_cmax > 0)
        ? 100.0 * (cmax - opt_cmax) / opt_cmax
        : 0.0;

    std::cout << std::left  << std::setw(12) << name
              << std::right << std::setw(8)  << cmax;
    if (opt_cmax > 0)
        std::cout << " [" << std::fixed << std::setprecision(1) << std::setw(6) << err << "%]";
    else
        std::cout << "         ";
    std::cout << "   " << std::fixed << std::setprecision(4) << ms << " ms\n";

    return {cmax, ms};
}

TaskVector generateTasks(int N, int pmin, int pmax, unsigned seed = 42) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(pmin, pmax);
    TaskVector v(N);
    std::generate(v.begin(), v.end(), [&]{ return dist(rng); });
    return v;
}

void runInstance(int m, int N, int pmin, int pmax) {
    TaskVector tasks = generateTasks(N, pmin, pmax);

    std::cout << "\n=== m=" << m << " N=" << N
              << " p=[" << pmin << "-" << pmax << "] ===\n";
    std::cout << "Tasks: " << tasks << "\n";
    std::cout << std::string(50, '-') << "\n";

    auto pd_res = benchmark("PD", [](TaskVector t){ return PD(t); }, tasks, m);
    int opt = pd_res.cmax;

    if (N <= 25)
        benchmark("CS", [](TaskVector t){ return CS(t); }, tasks, m, opt);
    else
        std::cout << "CS           N/A (N>25)\n";

    benchmark("LSA", [m](TaskVector t){ return LSA(t, m); }, tasks, m, opt);
    benchmark("LPT", [m](TaskVector t){ return LPT(t, m); }, tasks, m, opt);
}

void runInstancePTAS(int m, int N, int pmin, int pmax) {
    TaskVector tasks = generateTasks(N, pmin, pmax);
    
    Solution pd_sol = PD(tasks);
    int opt = Cmax(pd_sol, m);

    std::cout << "\n=== m=" << m << " N=" << N
              << " p=[" << pmin << "-" << pmax << "] ===\n";
    std::cout << "OPT (PD): " << opt << "\n";
    std::cout << std::string(50, '-') << "\n";

    std::vector<std::pair<int,int>> ptas_fracs = {{1,2},{2,3},{3,4}};
    for (auto [num, den] : ptas_fracs) {
        int k = std::max(1, N * num / den);
        if (k >= 25) {
            std::cout << "PTAS k=" << k << " (=" << num << "n/" << den
                      << ")   N/A (k>=25, 2^k niewykonalne)\n";
            continue;
        }
        std::string label = "PTAS " + std::to_string(num) + "n/"
                          + std::to_string(den) + " k=" + std::to_string(k);
        benchmark(label, [k](TaskVector t){ return PTAS(t, k); },
                  tasks, m, opt);
    }

    std::cout << std::string(50, '-') << "\n";

    // FPTAS K = 2, 3, 4
    for (int K : {2, 3, 4}) {
        std::string label = "FPTAS K=" + std::to_string(K);
        benchmark(label, [K](TaskVector t){ return FPTAS(t, K); },
                  tasks, m, opt);
    }
}

int main() {
    runInstancePTAS(2, 10,  1,  10);
    runInstancePTAS(2, 10, 10,  20);
    runInstancePTAS(2, 20,  1,  10);
    runInstancePTAS(2, 20, 10,  20);
    runInstancePTAS(2, 20, 50, 100);
    runInstancePTAS(2, 50,  1,  10);
    runInstancePTAS(2, 50, 10,  20);
    runInstancePTAS(2, 50, 50, 100);
}