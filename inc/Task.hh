#pragma once

#include <iostream>
#include <utility>

class Task{
    // int _task_id;        // propozycja zmiennej do identyfikacji tasków
    int _release_date;      // czas gotowości do realizacji zadania
    int _processing_time;   // czas wykonywania 
    int _due_date;          // oczekiwany czas realizacji zadania

public:
    Task() {};      
    // liczy w której chwili zadanie się wykona
    int calc_compl_time(int start_time) { return start_time > _release_date ? start_time + _processing_time : _release_date + _processing_time; }
    
    // liczy opóźnienie w wykonaniu zadania (może być ujemne, co oznacza, że zadania zostało zrealizowane przed czasem)
    int calc_lateness(int compl_time) { return compl_time - _due_date; }
    bool operator<(Task &other) const{ return _release_date < other._release_date; }
    void set_parameters(int release_date, int processing_time, int due_date) {_release_date = release_date; _processing_time = processing_time; _due_date = due_date;}
    int get_release_date() const{ return _release_date; }
    int get_processing_time() const{ return _processing_time; }
    int get_due_date() const{ return _due_date; }
};

inline
std::ostream &operator<<(std::ostream &ostr,const Task &task){
    return ostr << "[Release Date: " << task.get_release_date() << ", "
                << "Processing Time: " << task.get_processing_time() << ", "
                << "Due Date: " << task.get_due_date() << "]";
}
std::ostream &operator<<(std::ostream &ostr, const std::vector<Task> &v_Tasks){
    for(const Task &task : v_Tasks){
        ostr<<task<<std::endl;
    }
    return ostr;
}

inline
std::istream &operator>>(std::istream &istr,Task &task){
    int release_date, processing_time, due_date;
    istr >> release_date >> processing_time >> due_date;
    task.set_parameters(release_date, processing_time, due_date); 
    return istr;
}