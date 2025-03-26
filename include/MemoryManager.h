#ifndef MEMORYMANAGER.H
#define MEMORYMANAGER.h

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <set>
using namespace std;

enum class DeadlockStrategy {
    IGNORANCE,       
    DETECTION,       
    PREVENTION       
};

enum class ResourceType {
    MEMORY,
    DISK
};


struct ResourceRequest {
    int process_id;
    ResourceType type;
    size_t amount;
    chrono::steady_clock::time_point timestamp;
};

class ResourceAllocationTable {
private:
   
    unordered_map<int, unordered_map<ResourceType, size_t>> allocated_resources;    
    unordered_map<ResourceType, size_t> total_resources;  
    vector<ResourceRequest> pending_requests;    
    const chrono::seconds DEADLOCK_TIMEOUT{5};

public:
    ResourceAllocationTable() {        
        total_resources[ResourceType::MEMORY] = 1024 * 1024;
        total_resources[ResourceType::DISK] = 10 * 1024 * 1024;        
    }

    bool request_resource_with_detection(int process_id, ResourceType type, size_t amount) {        
        ResourceRequest request{
            process_id, 
            type, 
            amount, 
            chrono::steady_clock::now()
        };
        pending_requests.push_back(request);        
        if (detect_deadlock()) {        
            pending_requests.pop_back();
            cerr << "Potential deadlock detected. Request rejected.\n";
            return false;
        }        
        allocated_resources[process_id][type] += amount;
        total_resources[type] -= amount;
        cout <<"Process "<<process_id<<" allocated "<<amount<<" of "<<static_cast<int>(type)<< " after deadlock detection\n";
        return true;
    }

    bool detect_deadlock() {
        auto now = chrono::steady_clock::now();        
        for (const auto& request : pending_requests) {
            if (now - request.timestamp > DEADLOCK_TIMEOUT) {
                return true;
            }
        }     
        return false;
    }

    bool request_resource_with_prevention(int process_id, ResourceType type, size_t amount) {
       if (allocated_resources[process_id][type] + amount > total_resources[type]) {
            std::cerr << "Request would exceed maximum possible allocation\n";
            return false;
        }            
        bool can_allocate = true;
        for (const auto& [existing_type, existing_amount] : allocated_resources[process_id]) {
            if (static_cast<int>(existing_type) > static_cast<int>(type)) {
                can_allocate = false;
                break;
            }
        }
        if (!can_allocate) {
            cerr << "Resource allocation prevented to avoid potential deadlock\n";
            return false;
        }             
        allocated_resources[process_id][type] += amount;
        total_resources[type] -= amount;        
        cout << "Process "<<process_id<<" allocated "<<amount<<" of "<<static_cast<int>(type)<<" with prevention\n";
        return true;
    }
    void release_resources(int process_id) {
        for (auto& [type, amount] : allocated_resources[process_id]) {
            total_resources[type] += amount;
            std::cout << "Released " << amount 
                      << " of " << static_cast<int>(type) 
                      << " from Process " << process_id << std::endl;
        }
        allocated_resources.erase(process_id);
    }
};

#endif


