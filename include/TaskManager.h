#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <functional>
#include <vector>
#include <chrono>

class TaskManager {
private:
    struct Task {
        std::function<void()> func;
        std::time_t timestamp;
    };
    std::vector<Task> tasks;

public:
    TaskManager();
    void Add(std::function<void()> task, std::time_t timestamp);
    void RunTasks();
};

#endif // TASKMANAGER_H
