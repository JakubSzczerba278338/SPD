#pragma once

#include <vector>

class Task {
    int id;
    std::vector<int> jobs;
    int jobsSum;

    public:
    Task(int id, std::vector<int> vector): id(id), jobs(vector)
    {
        jobsSum = 0;
        for(const auto& job: jobs) jobsSum += job;
    }
    ~Task() {}

    std::vector<int> getJobs() const { return jobs;};
    int getId() const { return id;};
    int getJobsSum() const { return jobsSum;};
};
