#ifndef SCHEDULER_hpp
#define SCHEDULER_HPP

#include "Process.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>

using namespace std;

void fcfs(vector<Process> process){
    int time=0;
    double avg_tat =0,avg_wt=0;
    for(auto &p: process){
        if(time < p.arrivalTime){
            this_thread::sleep_for(chrono::milliseconds(p.arrivalTime-time));
            time = p.arrivalTime;
        }
        p.startTime = time;
        time += p.execution_time;
        p.completionTime = time;
        this_thread::sleep_for(chrono::milliseconds(p.execution_time));
        p.turnaroundTime = p.arrivalTime-p.completionTime;
        p.waitingTime = p.turnaroundTime - p.execution_time;
        avg_tat += p.turnaroundTime;
        avg_wt += p.waitingTime;
        p.displayInfo();
    }
    cout<<"Average Turnaround time: "<<avg_tat/process.size()<<" | Average waiting time: "<<avg_wt/process.size()<<endl;
};

void priorityScheduling(vector<Process> process){
    int time = 0;
    double avg_tat =0,avg_wt=0;
    for(int i=0;i<process.size();i++){
        for(int j=0;j<process.size()-1;j++){
            if(process[j].priority > process[j+1].priority){
                Process temp = process[j+1];
                process[j+1] = process[j];
                process[j] = temp;
            }
        }
    }
    for(int i=0;i<process.size();i++){
        process[i].displayInfo();
    }
};

#endif