#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

#include "Task.h"
#include "Algorithms.h"


// Czyta pojedynczy plik instancji do wektora zadań.
// Zwraca liczbę maszyn (proc_num) lub -1 przy błędzie.
int ReadFile(std::ifstream& file, TaskVector& Tasks);

// Struktura opisująca jedną wczytaną instancję.
struct Instance {
    std::string filename;   // nazwa pliku (bez ścieżki)
    int         numMachines;
    TaskVector  tasks;
};

// Wczytuje wszystkie pliki *.txt z podanego folderu.
// Zwraca wektor instancji posortowany alfabetycznie po nazwie pliku.
// Rzuca std::runtime_error jeśli folder nie istnieje.
std::vector<Instance> ReadFromFolder(const std::string& folderPath);