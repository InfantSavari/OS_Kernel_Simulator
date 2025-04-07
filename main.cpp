#include "Process.hpp"
#include "Scheduler.hpp"
#include "MemoryManager.h"
#include "IPC.h"
#include <iostream>
// #include <windows.h>
#include <vector>

using namespace std;

int main(){
    vector<Process> process;
    int pid,at,bt,priority;
    ResourceAllocationTable rat;
    int n1,n2,n3;
    int choice,p=0;
    IPC ipc;
    cout<<"|*********************************************************|"<<endl;
    cout<<"|                     OS Kernel Simulator                 |"<<endl;
    cout<<"|*********************************************************|"<<endl;
    while(true){
        cout<<"|                                                         |"<<endl;
        cout<<"|Main Menu:                                               |"<<endl;     
        cout<<"|         1. Create Process                               |"<<endl;
        cout<<"|         2. Start Execution                              |"<<endl;
        cout<<"|         3. Exit                                         |"<<endl;
        cout<<"|_________________________________________________________|"<<endl;
        cout<<endl;
        cout<<"Current state of System: "<<endl;
        rat.display_state();
        cout<<"|---------------------------------------------------------|"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;
        if(choice == 3){
            p=0;
            process.clear();
            cout << "\033[2J\033[H";
            cout<<"|*********************************************************|"<<endl;
            cout<<"|                     OS Kernel Simulator                 |"<<endl;
            cout<<"|*********************************************************|"<<endl; 
			continue;
        }
        // ResourceAllocationTable rat;
        if(choice == 1){
            cout<<"Enter number of process: ";
            int n;
            cin>>n;
        	for(int i=0;i<n;i++){
        		cout<<"Existing process details : "<<endl;
                               
                rat.display_state();            	
            	cout<<endl;
            	cout<<"Enter details of process: \n"<<endl;
            	cout<<"     1. Enter PID: ";
            	cin>>pid;
                for(auto& p : process){
                    if(p.pid == pid){
                        cout<<"Process with PID "<<pid<<" already exists!!!"<<endl;
                        continue;
                    }
                }
            	cout<<"     2. Enter process "<<pid<<" arrival time : ";
            	cin>>at;
            	cout<<"     3. Enter process "<<pid<<" burst time : ";
            	cin>>bt;
            	cout<<"     4. Enter maximum resource  needed for process [ A, B, C ]"<<pid<<" : ";
            	cin>>n1>>n2>>n3;
            	int m1,m2,m3;
                vector<int> max = {n1,n2,n3};
            	cout<<"     5. Enter request from process [ A, B, C ]"<<pid<<" : ";
            	cin>>m1>>m2>>m3;
                vector<int> req = {m1,m2,m3};
            	Process temp(pid,at,bt);
            	
            	rat.set_max_demand(pid,max);
            	bool t = rat.request_resources(pid,req);
                if(!t){
                    cout<<"Process "<<pid<<" is discarded!!!"<<endl;
                    continue;
                }
            	process.push_back(temp);
			}
            cout<<"_________________________________________________________"<<endl;
            continue;
        }
        if(choice == 2){
            cout<<"Process scheduling algorithm: "<<endl;
            cout<<"     1. First Come First Serve\n     2. Priority Scheduling\n        3. ShortestRemainingJobFirst\n      4.roundRobin"<<endl;
            // cout<<"     5. Let kernel decide: "<<endl;
            cin>>choice;
            if(choice == 1){
                fcfs(process, rat,ipc);
                printResults(process);
                
            }
            else if(choice == 2){
                
                cout<<"Enter priority for Process "<<endl;
                for(auto& p : process){
                    cout<<"PID "<<p.pid<<" Priority: "<<endl;
                    cin>>p.priority;
                }
                priorityScheduling(process,rat,ipc);
                
            }
            else if(choice == 3){
                shortestJobFirst(process, rat,ipc);
                
            }
            else if (choice == 4){
                int qt;
                cout<<"Enter quantum time: ";
                cin>>qt;
                roundRobin(process,qt,rat,ipc);
                printResults(process);
                
            }
            
            cout<<"_________________________________________________________"<<endl;
        }

    }
}