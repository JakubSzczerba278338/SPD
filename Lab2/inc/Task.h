#pragma once

#include <vector>
#include <iostream>
#include <sstream>
class Task {
    int id;
    std::vector<int> jobs;
    int jobsSum;

    public:
    Task(int id = -1) : id(id) {}
    Task(int id, std::vector<int> vector): id(id), jobs(vector)
    {
        setJobsSum();
    }
    ~Task() {}

    const std::vector<int>& getJobs() const { return jobs;};
    int getId() const { return id;};
    int getJobsSum() const { return jobsSum;};
    void addJob(int idx,int job_time) { if(idx >= jobs.size()) jobs.push_back(job_time); else jobs[idx] = job_time; }
    void changeId(int Id) {id = Id;}
    void setJobsSum(){ jobsSum = 0;
        for(const auto& job: jobs) jobsSum += job;}
};


inline std::istream& operator >> (std::istream& istrm, Task& task){
    int num{},value{};
    std::string line;
    std::getline(istrm,line);
    std::istringstream line_stream(line);
    while(line_stream>>num){
        line_stream>>value;
        task.addJob(num, value);
    }
    task.setJobsSum();
    return istrm;
}

inline std::ostream& operator <<(std::ostream& ostrm, Task& task){

    for(auto & job : task.getJobs()){
        ostrm<<job<<" ";
    }
    return ostrm;
}