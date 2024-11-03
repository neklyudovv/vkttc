#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <functional>
#include <unordered_map>
#include <thread>
#include <ctime>

class TaskManager {
private:
    struct Task {
        std::function<void()> func;
        std::time_t timestamp;
    };
    int id= 0;

    std::unordered_map<int, Task> tasks;
    std::thread taskThread;

public:
    TaskManager();
    ~TaskManager();

    int Add(std::function<void()> task, std::time_t timestamp);
    void startThread();
    void eraseTask(int id);
};

#endif // TASKMANAGER_H
