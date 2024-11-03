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
    while(!tasks.empty()){
        std::time_t now = std::time(nullptr);
        for (auto it = tasks.begin(); it != tasks.end();)
            if (it->second.timestamp <= now) {
                it->second.func();
                eraseTask(it->first);
                it = tasks.find(it->first);
            } else ++it;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void TaskManager::eraseTask(int id){
    tasks.erase(id);
}

int TaskManager::Add(std::function<void()> task, std::time_t timestamp){
    tasks[id] = {task, timestamp};
    return id++;
}