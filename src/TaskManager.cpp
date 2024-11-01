#include "../include/TaskManager.h"
#include <iostream>


TaskManager::TaskManager(){}

void TaskManager::Add(std::function<void()> task, std::time_t timestamp){
    tasks.push_back({task, timestamp});
}

void TaskManager::RunTasks() {
    std::time_t now = std::time(nullptr);
    for (auto it = tasks.begin(); it != tasks.end();) {
        if (it->timestamp <= now) {
            it->func();
            it = tasks.erase(it);
        } else {
            ++it;
        }
    }
}