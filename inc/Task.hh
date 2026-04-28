 #pragma once

#include <iostream>
#include <utility>
#include <vector>

class Task{
    // int _task_id;        // propozycja zmiennej do identyfikacji tasków
    int _release_date;      // czas gotowości do realizacji zadania
    int _processing_time;   // czas wykonywania 
    int _due_date;          // oczekiwany czas realizacji zadania
    mutable int _time_processed;

public:
    Task() {};      
    // liczy w której chwili zadanie się wykona
    int calc_compl_time(int start_time) const { return start_time > _release_date ? start_time + _processing_time : _release_date + _processing_time; }
    // przeciążenie dla wywłaszczającego
    int calc_compl_time(int start_time,int processed_time) const{/*std::cout<<_time_processed<<std::endl;*/return start_time > _release_date ? start_time + processed_time : _release_date + processed_time;} 
    // liczy opóźnienie w wykonaniu zadania (może być ujemne, co oznacza, że zadania zostało zrealizowane przed czasem)
    int calc_lateness(int compl_time) const { return compl_time - _due_date; }
    bool operator<(Task &other) const { return _release_date == other._release_date ? (_processing_time == other._processing_time ? 
                                                                                            _due_date < other._due_date : _processing_time < other._processing_time) :_release_date < other._release_date;} // generalnie chodzi o to że jak masz równe release date'y to nie przeszukasz wszystkich permutacji i complete search da zły wynik
    //bool operator<(Task &other) const { return _due_date < other._due_date;}
    void set_parameters(int release_date, int processing_time, int due_date,int time_processed = 0) {_release_date = release_date; _processing_time = processing_time; _due_date = due_date; _time_processed = time_processed;}
    void change_time_processed(int processed_time) const {if(int temp = _time_processed + processed_time ; temp <= _processing_time){_time_processed = temp;} else {_time_processed = _processing_time;};} // może inne nazwy lollll
    bool is_finished() const {return _time_processed == _processing_time ? true : false;}
    int get_release_date() const { return _release_date; }
    int get_processing_time() const { return _processing_time; }
    int get_due_date() const { return _due_date; }
    int get_time_processed() const {return _time_processed;}
};

inline
std::ostream &operator<<(std::ostream &ostr,const Task &task){
    return ostr << "[Release Date: " << task.get_release_date() << ", "
                << "Processing Time: " << task.get_processing_time() << ", "
                << "Due Date: " << task.get_due_date() << "]";
}
inline
std::ostream &operator<<(std::ostream &ostr, const std::vector<Task> &v_Tasks){
    for(const Task &task : v_Tasks){
        ostr<<task<<std::endl;
    }
    return ostr;
}
inline
std::ostream& operator<<(std::ostream& ostr,const std::pair<Task,int> &r_Pair){
    return ostr<<"("<<r_Pair.first<<", "<<r_Pair.second<<")";
}
inline
std::ostream& operator<<(std::ostream &ostr, const std::vector<std::pair<Task,int>> &v_Result){
    for(const std::pair<Task,int> &r_Pair : v_Result){
        ostr<<r_Pair<<std::endl;
    }
    return ostr;
}

inline
std::istream &operator>>(std::istream &istr,Task &task){
    int release_date, processing_time, due_date;
    istr >> processing_time >> release_date>> due_date;
    task.set_parameters(release_date, processing_time, due_date); 
    return istr;
}