#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <mutex>

using namespace std;

struct ResourceRequest {
    int process_id;
    vector<int> request;
    chrono::steady_clock::time_point timestamp;
};

class ResourceAllocationTable {
private:
    int resources = 3;  // Number of resource types
    int processes;  // Number of processes
    
    vector<int> total_resources;
    vector<int> available;      
    unordered_map<int, vector<int>> allocated;
    unordered_map<int, vector<int>> max_need;
    vector<ResourceRequest> pending_requests;
    mutex table_mutex;
    int problem_id;

public:
    ResourceAllocationTable(int num_processes) {
        processes = num_processes;
        total_resources = {8, 8, 8};  
        available = total_resources;
    }

    void set_max_demand(int process_id, vector<int> max_demand) {
        if (process_id < 0 || process_id >= processes) {
            cerr << "Invalid process ID\n";
            return;
        }
        max_need[process_id] = max_demand;
        allocated[process_id] = vector<int>(resources, 0);
    }

    bool is_safe_state(int process_id, vector<int> request) {
        vector<int> temp_available = available;
        unordered_map<int, vector<int>> temp_allocated = allocated;

        for (int i = 0; i < resources; i++) {
            temp_available[i] -= request[i];
            temp_allocated[process_id][i] += request[i];
        }

        vector<int> work = temp_available;
        vector<bool> finish(processes, false);

        while (true) {
            bool found = false;
            for (int i = 0; i < processes; i++) {
                if (!finish[i]) {
                    bool can_satisfy = true;
                    for (int j = 0; j < resources; j++) {
                        if (max_need[i][j] - temp_allocated[i][j] > work[j]) {
                            can_satisfy = false;
                            break;
                        }
                    }
                    if (can_satisfy) {
                        for (int j = 0; j < resources; j++)
                            work[j] += temp_allocated[i][j];
                        finish[i] = true;
                        found = true;
                    }
                }
            }
            if (!found) break;
        }

        for (int i = 0; i < processes; i++) {
            if (!finish[i]) {
                cout << "Unsafe state detected with Process " << i << endl;
                return false;
            }
        }
        return true;
    }

    bool request_resources(int process_id, vector<int> request) {
        lock_guard<mutex> lock(table_mutex);

        if (process_id < 0 || process_id >= processes) {
            cerr << "Invalid process ID\n";
            return false;
        }

        for (int i = 0; i < resources; i++) {
            if (request[i] > max_need[process_id][i]) {
                cerr << "Request exceeds maximum demand for Process " << process_id << endl;
                return false;
            }
        }

        for (int i = 0; i < resources; i++) {
            if (request[i] > available[i]) {
                cerr << "Not enough resources available for Process " << process_id << endl;
                return false;
            }
        }

        if (!is_safe_state(process_id, request)) {
            cerr << "Request denied to prevent deadlock.\n";
            return false;
        }

        for (int i = 0; i < resources; i++) {
            available[i] -= request[i];
            allocated[process_id][i] += request[i];
        }

        cout << "Process " << process_id << " allocated resources: ";
        for (int i = 0; i < resources; i++)
            cout << request[i] << " ";
        cout << endl;

        return true;
    }

    void release_resources(int process_id) {
        lock_guard<mutex> lock(table_mutex);

        if (process_id < 0 || process_id >= processes) {
            cerr << "Invalid process ID\n";
            return;
        }

        cout << "Process " << process_id << " releasing resources: ";
        for (int i = 0; i < resources; i++) {
            cout << allocated[process_id][i] << " ";
            available[i] += allocated[process_id][i];
            allocated[process_id][i] = 0;
        }
        cout << endl;
    }

    void display_state() {
        lock_guard<mutex> lock(table_mutex);

        cout << "\nResource State\n";
        cout << "Available: ";
        for (int i = 0; i < resources; i++) {
            cout << available[i] << " ";
        }
        cout << "\nAllocated:\n";
        for (const auto& pair : allocated) {
            int pid = pair.first;
            cout << "P" << pid << ": ";
            for (int j = 0; j < resources; j++) {
                cout << allocated[pid][j] << " ";
            }
            cout << endl;
        }
    }
};




#endif

// int main() {
//     vector<int> total_resources = {8, 8, 8};  // 3 resource types, each with 8 instances
//     int num_processes = 3;

//     ResourceAllocationTable resource_table(num_processes, total_resources);

//     // Set maximum demand for each process
//     resource_table.set_max_demand(0, {5, 3, 2});
//     resource_table.set_max_demand(1, {3, 2, 2});
//     resource_table.set_max_demand(2, {4, 3, 3});

//     resource_table.display_state();

//     // Requests from processes
//     resource_table.request_resources(0, {1, 2, 2});  // P0
//     resource_table.request_resources(1, {2, 1, 1});  // P1
//     resource_table.request_resources(2, {3, 2, 2});  // P2

//     resource_table.display_state();

//     // Release resources
//     resource_table.release_resources(0);
//     resource_table.release_resources(1);
//     resource_table.release_resources(2);

//     resource_table.display_state();

//     return 0;
// }