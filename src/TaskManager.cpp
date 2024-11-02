#include "../include/TaskManager.h"
#include <ctime>
#include <thread>
#include <chrono>

TaskManager::TaskManager(){
    startThread();
}
TaskManager::~TaskManager(){
    if (taskThread.joinable())
        taskThread.join();
}

void TaskManager::startThread(){
    taskThread = std::thread([this](){ // анонимная лямбда функция для обращения к tasks
    while(true){
        std::time_t now = std::time(nullptr);
        for (auto it = tasks.begin(); it != tasks.end();)
            if (it->timestamp <= now) {
                it->func();
                eraseTask(it);
            } else ++it;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void TaskManager::eraseTask(std::vector<Task>::iterator it){
    tasks.erase(it);
}

void TaskManager::Add(std::function<void()> task, std::time_t timestamp){
    tasks.push_back({task, timestamp});
}