#include "Utils.h"

#include <algorithm>
#include <stdexcept>

namespace fs = std::filesystem;


int ReadFile(std::ifstream& File, TaskVector& Tasks){
    int task_num{},proc_num{},task_id{1};
    File >> task_num >> proc_num;
    std::string dummy;
    std::getline(File,dummy);
    
    Task task{task_id };
    Tasks.resize(task_num);
    while(task_id <= task_num && File >> task ){
        Tasks[task_id-1] = task;
        task_id++;
        task.changeId(task_id );
    }
    
    return proc_num;
}


std::vector<Instance> ReadFromFolder(const std::string& folderPath) {
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        throw std::runtime_error("ReadFromFolder: folder nie istnieje lub nie jest katalogiem: " + folderPath);
    }

    // Zbierz ścieżki do wszystkich plików .txt
    std::vector<fs::path> paths;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            paths.push_back(entry.path());
        }
    }

    // Sortuj alfabetycznie po nazwie pliku
    std::sort(paths.begin(), paths.end(), [](const fs::path& a, const fs::path& b) {
        return a.filename() < b.filename();
    });

    std::vector<Instance> instances;
    instances.reserve(paths.size());

    for (const auto& p : paths) {
        std::ifstream file(p);
        if (!file.is_open()) {
            // Pomijamy pliki, których nie można otworzyć
            continue;
        }

        Instance inst;
        inst.filename    = p.filename().string();
        inst.numMachines = ReadFile(file, inst.tasks);
        instances.push_back(std::move(inst));
    }

    return instances;
}