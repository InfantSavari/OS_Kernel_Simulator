#include "Process.hpp"
#include "Scheduler.hpp"
#include "MemoryManager.h"
#include "IPC.h"
#include <iostream>
#include <vector>

using namespace std;

int main(){
    vector<Process> process;
    int pid,at,bt,priority;
    ResourceAllocationTable rat(5);
    int n1,n2,n3;
    int choice,p=0;
    IPC ipc;
    cout<<"                 *********************************************************"<<endl;
    cout<<"                                     OS Kernel Simulator                 \n\n"<<endl;
    cout<<"                 *********************************************************"<<endl;
    while(true){
        
        cout<<"                 1. Create Process \n
                                2. Start Execution\n
                                3. Exit"<<endl;
        
        cin>>choice;
        if(choice == 3){
            p=0;
            process.clear();
			continue;
        }
        // ResourceAllocationTable rat;
        if(choice == 1){
            cout<<"                 Enter number of process: ";
            int n;
            cin>>n;
        	for(int i=0;i<n;i++){
        		cout<<"                 Existing process details : PID  |  Arrival Time  |  Burst Time | resources( A ) | resources( B ) | resources( C ) "<<endl;
            	rat.display_state();            	
            	cout<<endl;
            	cout<<"Enter details of process: \n"<<endl;
            	cout<<"     1. Enter PID: ";
            	cin>>pid;
            	cout<<"     2. Enter process "<<pid<<" arrival time : ";
            	cin>>at;
            	cout<<"     3. Enter process "<<pid<<" burst time : ";
            	cin>>bt;
            	cout<<"     4. Enter maximum resource  needed for process [ A, B, C ]"<<pid<<" : ";
            	cin>>n1>>n2>>n3;
            	int m1,m2,m3;
            	cout<<"     5. Enter request from process [ A, B, C ]"<<pid<<" : ";
            	cin>>m1>>m2>>m3;
            	Process temp(pid,at,bt);
            	temp.priority =p++;
            	rat.set_max_demand(pid,{n1,n2,n3});
            	rat.request_resources(pid,{m1,m2,m3});
			}
            
            continue;
        }
        if(choice == 2){
            cout<<"Process scheduling algorithm: "<<endl;
            cout<<"     1. First Come First Serve\n     2. Priority Scheduling\n        3. ShortestRemainingJobFirst\n      4.roundRobin"<<endl;
            cout<<"     5. Let kernel decide: "<<endl;
            cin>>choice;
            if(choice == 1){
                fcfs(process, rat,ipc);
                printResults(process);
                continue;
            }
            else if(choice == 2){
                
                cout<<"Enter priority for Process "<<endl;
                for(auto& p : process){
                    cout<<"PID "<<p.pid<<" Priority: "<<endl;
                    cin>>p.priority;
                }
                priorityScheduling(process,rat,ipc);
                continue;
            }
            else if(choice == 3){
                shortestJobFirst(process, rat,ipc);
                continue;
            }
            else if (choice == 4){
                int qt;
                cout<<"Enter quantum time: ";
                cin>>qt;
                roundRobin(process,qt,rat,ipc);
                printResults(process);
                continue;
            }
            else{
                // MultiLevelQueueScheduler mqs;
                // for(auto &i : process){
                //     mqs.addProcess(i);
                // }
                // mqs.executeProcesses();
                continue;
            }
        }

    }
}