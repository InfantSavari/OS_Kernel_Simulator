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

void priorityScheduling(vector<Process>& processes) {
    
    
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime == b.arrivalTime)
            return a.priority < b.priority; // Lower number = higher priority
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0;

    for (auto& p : processes) {
        p.state = ProcessState::READY;

        if (time < p.arrivalTime) {
            time = p.arrivalTime;
        }

        p.state = ProcessState::RUNNING;
        cout << "[Process " << p.pid << "] Running...\n";
        this_thread::sleep_for(chrono::milliseconds(p.executionTime));

        p.completionTime = time + p.executionTime;
        time += p.executionTime;

        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.executionTime;
        p.state = ProcessState::TERMINATED;

        cout << "[Process " << p.pid << "] Completed at " << p.completionTime << "\n";
    }
};

void shortestJobFirst(vector<Process>& processes) {
    

    int n = processes.size();
    int completed = 0, time = 0, minIndex = -1;
    bool found;
    
    while (completed < n) {
        found = false;
        int minRemaining = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0 && processes[i].remainingTime < minRemaining) {
                minRemaining = processes[i].remainingTime;
                minIndex = i;
                found = true;
            }
        }

        if (!found) {
            time++;
            continue;
        }

        processes[minIndex].state = ProcessState::RUNNING;
        this_thread::sleep_for(chrono::milliseconds(1)); // Simulate execution
        processes[minIndex].remainingTime--;
        time++;

        if (processes[minIndex].remainingTime == 0) {
            completed++;
            processes[minIndex].completionTime = time;
            processes[minIndex].turnaroundTime = processes[minIndex].completionTime - processes[minIndex].arrivalTime;
            processes[minIndex].waitingTime = processes[minIndex].turnaroundTime - processes[minIndex].executionTime;
            processes[minIndex].state = ProcessState::TERMINATED;
        }
    }
};

void roundRobin(vector<Process>& processes, int quantum) {
    cout << "\n🔹 Round Robin Scheduling (Quantum = " << quantum << "ms)\n";

    int n = processes.size();
    int time = 0, completed = 0;
    queue<int> q;
    vector<bool> inQueue(n, false);

    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    q.push(0);
    inQueue[0] = true;

    while (completed < n) {
        int index = q.front();
        q.pop();
        inQueue[index] = false;

        processes[index].state = ProcessState::RUNNING;
        int execTime = min(quantum, processes[index].remainingTime);
        time += execTime;
        processes[index].remainingTime -= execTime;

        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && processes[i].arrivalTime <= time && processes[i].remainingTime > 0) {
                q.push(i);
                inQueue[i] = true;
            }
        }

        if (processes[index].remainingTime > 0) {
            q.push(index);
            inQueue[index] = true;
            processes[index].state = ProcessState::READY;
        } else {
            completed++;
            processes[index].completionTime = time;
            processes[index].turnaroundTime = processes[index].completionTime - processes[index].arrivalTime;
            processes[index].waitingTime = processes[index].turnaroundTime - processes[index].executionTime;
            processes[index].state = ProcessState::TERMINATED;
        }
    }
};


void printResults(vector<Process>& processes) {
    float avgWaiting = 0, avgTurnaround = 0;
    cout << "\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for (auto& p : processes) {
        this_thread::sleep_for(chrono::milliseconds(process[i].execution_time));
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" << p.completion
             << "\t\t" << p.turnaround << "\t\t" << p.waiting << "\n";
        avgWaiting += p.waiting;
        avgTurnaround += p.turnaround;
    }
    cout << "\nAverage Waiting Time: " << avgWaiting / processes.size() << " ms";
    cout << "\nAverage Turnaround Time: " << avgTurnaround / processes.size() << " ms\n";
};

#endif