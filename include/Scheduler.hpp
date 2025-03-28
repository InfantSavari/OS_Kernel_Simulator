#ifndef SCHEDULER_hpp
#define SCHEDULER_HPP

#include "Process.hpp"
#include "MemoryManager.h"
#include "IPC.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <climits>
#include <unistd.h>


using namespace std;

long long int vp=INT_MAX;



    
void fcfs(vector<Process> process,ResourceAllocationTable &rat, IPC &ipc){
    int time=0;
    double avg_tat =0,avg_wt=0;
    cout<<"\n         First Come First Serve      \n"<<endl;
    for(auto &p: process){
        if(time < p.arrivalTime){
            this_thread::sleep_for(chrono::milliseconds((p.arrivalTime-time)*100));
            time = p.arrivalTime;
        }
        ipc.writeMessage();
        p.startTime = time;
        time += p.execution_time;
        p.completionTime = time;
        this_thread::sleep_for(chrono::milliseconds(p.execution_time));
        p.turnaroundTime = p.arrivalTime-p.completionTime;
        p.waitingTime = p.turnaroundTime - p.execution_time;
        avg_tat += p.turnaroundTime;
        avg_wt += p.waitingTime;
        rat.release_resources(p.pid);
        ipc.readMessage();
    }
    cout<<"Average Turnaround time: "<<avg_tat/process.size()<<" | Average waiting time: "<<avg_wt/process.size()<<endl;
};

void priorityScheduling(vector<Process>& processes, ResourceAllocationTable &rat, IPC &ipc) {
    
    cout<<"\n       Priority Scheduling        \n"<<endl;
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime == b.arrivalTime)
            return a.priority < b.priority;
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0;

    for (auto& p : processes) {
        p.state = ProcessState::READY;

        if (time < p.arrivalTime) {
            time = p.arrivalTime;
        }
        ipc.writeMessage();
        p.state = ProcessState::RUNNING;
        cout << "[Process " << p.pid << "] Running...\n";
        this_thread::sleep_for(chrono::milliseconds(p.executionTime));
        p.completionTime = time + p.executionTime;
        time += p.executionTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.executionTime;
        p.state = ProcessState::TERMINATED;
        ipc.readMessage();
        rat.release_resources(p.pid);
        sleep(p.execution_time);
        cout << "[Process " << p.pid << "] Completed at " << p.completionTime << "\n";
    }
};

void shortestJobFirst(vector<Process>& process,ResourceAllocationTable &rat;) {
    int n = process.size();
    int completed = 0, time = 0, minIndex = -1;
    bool found;
    cout<<"\n         Shortest Job First      \n"<<endl;
    while (completed < n) {
        found = false;
        int minRemaining = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (process[i].arrivalTime <= time && process[i].remainingTime > 0 && process[i].remainingTime < minRemaining) {
                minRemaining = process[i].remainingTime;
                minIndex = i;
                found = true;
            }
        }

        if (!found) {
            time++;
            continue;
        }
        ipc.writeMessage();
        process[minIndex].state = ProcessState::RUNNING;
        this_thread::sleep_for(chrono::milliseconds(1));
        process[minIndex].remainingTime--;
        time++;

        if (process[minIndex].remainingTime == 0) {
            completed++;
            process[minIndex].completionTime = time;
            process[minIndex].turnaroundTime = process[minIndex].completionTime - process[minIndex].arrivalTime;
            process[minIndex].waitingTime = process[minIndex].turnaroundTime - process[minIndex].executionTime;
            rat.release_resources(process[minIndex].pid);
            cout << "[Process " << process[minIndex].pid<< "] Running...\n";
            sleep(prcess[minIndex].execution_time);
            ipc.readMessage();
            cout << "[Process " << process[minIndex].pid << "] Completed at " << process[minIndex].completionTime << "\n";
            process[minIndex].state = ProcessState::TERMINATED;
        }
    }
};

void roundRobin(vector<Process>& processes, int quantum,ResourceAllocationTable &rat) {
    cout << "\n Round Robin Scheduling (Quantum = " << quantum << "ms)\n";

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
        ipc.writeMessage();
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
            rat.release_resources(processes[index].pid);
            cout << "[Process " << process[index].pid<< "] Running...\n";
            sleep(prcess[index].execution_time);
            ipc.readMessage();
            cout << "[Process " << process[index].pid << "] Completed at " << process[index].completionTime << "\n";
            processes[index].state = ProcessState::TERMINATED;
        }
    }
};


void printResults(vector<Process>& processes) {
    float avgWaiting = 0, avgTurnaround = 0;
    
    for (auto& p : processes) {
        cout << "[Process " << p.pid << "] Running...\n";
        sleep(prcess[i].execution_time);
        cout << "[Process " << p.pid << "] Completed at " << p.completionTime << "\n";
        avgWaiting += p.waiting;
        avgTurnaround += p.turnaround;
    }
    cout << "\nAverage Waiting Time: " << avgWaiting / processes.size() << " ms";
    cout << "\nAverage Turnaround Time: " << avgTurnaround / processes.size() << " ms\n";
};

class MultiLevelQueueScheduler {
    private:
        queue<Process> highPriorityQueue;
        queue<Process> midPriorityQueue;
        queue<Process> lowPriorityQueue;
    
    public:
        void addProcess(const Process& process) {
            if (process.priority <= 3)
                highPriorityQueue.push(process);
            else if (process.priority <= 6)
                midPriorityQueue.push(process);
            else
                lowPriorityQueue.push(process);
        }
    
        void executeProcesses() {
            fcfs(highPriorityQueue);
            printResults(highPriorityQueue);
            roundRobin(midPriorityQueue,2);
            printResults(midPriorityQueue);
            priorityScheduling(lowPriorityQueue);

        }    
};

#endif