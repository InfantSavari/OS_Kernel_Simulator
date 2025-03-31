#ifndef IPC_H
#define IPC_H

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <semaphore.h>
#include <sys/wait.h>

#define SHM_NAME "/multi_process_shm"
#define SEM_NAME "/shm_sem"
#define SHM_SIZE 1024
using namespace std;

class IPC {
private:
    int shm_fd;
    char* shm_ptr;
    sem_t* sem;

public:
    IPC(){
        shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) {
            cerr << "[SharedMemoryIPC] Failed to create shared memory!\n";
            exit(1);
        }
        ftruncate(shm_fd, SHM_SIZE);
        shm_ptr = static_cast<char*>(mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

        if (shm_ptr == MAP_FAILED) {
            cerr << "[SharedMemoryIPC] Failed to map shared memory!\n";
            exit(1);
        }

        sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    }
    
    ~IPC() {
        munmap(shm_ptr, SHM_SIZE);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        sem_close(sem);
        sem_unlink(SEM_NAME);
    };
    static string timeToString(time_t timeValue) {
        std::ostringstream oss;
        std::tm* timeInfo = std::localtime(&timeValue); // Convert time_t to tm structure
        oss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS
        return oss.str();
    }
    void writeMessage(){
        sem_wait(sem);
        
        strncpy(shm_ptr, message.c_str(), SHM_SIZE);
        
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now);
        string message = IPC.timeToString(now_c);
        strncpy(shm_ptr, message.c_str(), SHM_SIZE);
        sem_post(sem);
    };
    void readMessage(){
        sem_wait(sem);
        cout << "[SharedMemoryIPC] Message read: " << shm_ptr << "\n";
        sem_post(sem);
    }
};
#endif


