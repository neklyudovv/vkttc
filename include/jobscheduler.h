#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <functional>
#include <unordered_map>
#include <thread>
#include <ctime>
#include <mutex>
#include <condition_variable>

class jobscheduler {
private:
    struct Task {
        std::function<void()> func;
        std::time_t timestamp;
    };
    int id= 0;

    std::unordered_map<int, Task> tasks;
    std::thread taskThread;
    std::mutex m;
    std::condition_variable cv;
    bool running = true;

public:
    jobscheduler();
    ~jobscheduler();

    void startThread();
    int Add(std::function<void()> task, std::time_t timestamp);
    void eraseTask(int id);
};

#endif // TASKMANAGER_H
