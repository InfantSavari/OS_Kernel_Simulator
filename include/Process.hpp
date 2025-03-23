#ifndef Process_hpp
#define Process_hpp

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
using namespace std;

enum class ProcessState{
    New,
    Ready,
    Running,
    BLocked,
    Terminated
};

class Process {
    public:
    int pid;
    int execution_time;
    ProcessState currentState;
    int arrivalTime, remainingTime, priority;
    int startTime, completionTime, waitingTime, turnaroundTime;
    bool completed;

    Process(int pid, int at,int execution_time,int rt,int prior){
        this->pid = pid;
        this->execution_time = execution_time;
        this->currentState = ProcessState::New;
        this->arrivalTime = at;
        this->remainingTime = rt;
        this->priority = prior;
    }
    
    void displayInfo(){
        cout<<"Process ID: "<<pid<<" | Completed at : "<<completionTime<<endl;
    }
};

#endif