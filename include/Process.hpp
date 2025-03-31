#ifndef Process_hpp
#define Process_hpp

#include "MemoryManager.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
using namespace std;

enum class ProcessState{
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};


class Process {
    public:
    int pid;
    int execution_time;
    ProcessState currentState;
    
    long long int virtualpage;
     int resources_allocated;
    int arrivalTime, remainingTime, priority,max_wait_time,wait_time;
    int startTime, completionTime, waitingTime, turnaroundTime;
    bool completed;

    Process(int pid, int at,int execution_time){
        this->pid = pid;
        this->execution_time = execution_time;
        this->currentState = ProcessState::NEW;
        this->arrivalTime = at;
        this->remainingTime = execution_time;
        
 
        
        max_wait_time = 100;
        wait_time=0;
    }
    
    void displayInfo(){
        cout<<"Process ID: "<<pid<<" | Arrival Time : "<<arrivalTime<<" | Burst Time: "<<execution_time<<" | priority: "<<priority<<endl;
    }
    void execute(){
        cout<<"Process "<<pid<<"completed at "<<completionTime<<endl; 
    }
    // bool operator <(const Process& other)const{
    //     return priority < other.priority;
    // }

};

#endif