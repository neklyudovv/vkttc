#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <functional>
#include <vector>
#include <thread>
#include <ctime>

class TaskManager {
private:
    struct Task {
        std::function<void()> func;
        std::time_t timestamp;
    };
    std::vector<Task> tasks;
    std::thread taskThread;

public:
    TaskManager();
    ~TaskManager();

    void Add(std::function<void()> task, std::time_t timestamp);
    void startThread();
   // void RunTasks();
};

#endif // TASKMANAGER_H
