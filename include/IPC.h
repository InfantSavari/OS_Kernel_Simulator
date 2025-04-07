#ifndef IPC_H
#define IPC_H

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

class IPC {
    private:
        int current_pid;
         ofstream file;  
    
    public:
        IPC() {
            current_pid = 0;
            ofstream file("message.txt", ios::app);
        }     
        
        
        void writeMessage(int pid) {
            ofstream file("message.txt", std::ios::app);
            current_pid = pid;
                      
            if (file.is_open()) {
                file << "Process " << current_pid << " entered CS\n";
                file<<"Process "<<current_pid<<" used CS"<<endl;
                file.close();
            } else {
                cout << "Unable to open file\n";
            }
        }
        
        void readMessage() {
            ifstream file("message.txt");
            string line;
            
            if (file.is_open()) {
                while (getline(file, line)) {
                    cout << line << endl;
                }
                file.close();
            } else {
                cout << "Unable to open file\n";
            }
        }
    };
    

#endif
