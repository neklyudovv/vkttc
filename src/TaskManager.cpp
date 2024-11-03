#include "../include/TaskManager.h"
#include <ctime>
#include <thread>
#include <chrono>
#include <condition_variable>

TaskManager::TaskManager(){
    startThread();
}
TaskManager::~TaskManager(){
    {
      std::lock_guard<std::mutex> lock(m);
      running = false;
    }
    cv.notify_all();
    if (taskThread.joinable())
        taskThread.join();
}

void TaskManager::startThread(){
    taskThread = std::thread([this](){ // анонимная лямбда функция для обращения к tasks
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]() {return !tasks.empty() || !running;});
        if (!running && tasks.empty()) return;

        std::time_t now = std::time(nullptr);
        for (auto it = tasks.begin(); it != tasks.end();)
            if (it->second.timestamp <= now) {
                it->second.func();
                it = tasks.erase(it);
                break;
            } else ++it;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void TaskManager::eraseTask(int id){
    std::lock_guard<std::mutex> lock(m);
    tasks.erase(id);
}

int TaskManager::Add(std::function<void()> task, std::time_t timestamp){
    {
        std::lock_guard<std::mutex> lock(m);
        tasks[id] = {task, timestamp};
    }
    cv.notify_all();
    return id++;
}