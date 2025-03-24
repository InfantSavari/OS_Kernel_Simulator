
#ifndef IPC_HPP
#define IPC_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

class MessageQueue {
private:
    queue<string> queue;
    mutex mtx;
    condition_variable cv;
public:
    void send(const string& message){
        lock_guard<std::mutex> lock(mtx);
        queue.push(message);
        cv.notify_one();
    };
    string receive(){
        unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !queue.empty(); });
        string message = queue.front();
        queue.pop();
        return message;
    };
};

#endif 




