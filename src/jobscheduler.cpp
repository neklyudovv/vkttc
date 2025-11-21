#include "../include/jobscheduler.h"
#include <ctime>
#include <thread>
#include <condition_variable>

jobscheduler::jobscheduler(){
    startThread();
}
jobscheduler::~jobscheduler(){
    {
        std::lock_guard<std::mutex> lock(m);
        running = false;
    }
    cv.notify_all();
    if (taskThread.joinable())
        taskThread.join();
}

/**
* starts new thread in stated time
*
* this method starts a thread, that constantly checks for scheduled workers and
* if timestamp equals current time, it will start it in diff thread
*
* using mutex and condition variable for safe data usage
*/

void jobscheduler::startThread(){
    taskThread = std::thread([this](){
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]() {return !tasks.empty() || !running;});
        if (!running && tasks.empty()) return;

        std::time_t now = std::time(nullptr);
        for (auto it = tasks.begin(); it != tasks.end();)
            if (it->second.timestamp <= now) {
                std::thread currTask = std::thread([func=it->second.func](){func();});
                currTask.detach();
                // auto collecting resources but
                // better not use with functions that needs to be strong controlled
                it = tasks.erase(it); // iterator so it doesnt point to deleted element
                break;
            } else ++it;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void jobscheduler::eraseTask(int id){
    std::lock_guard<std::mutex> lock(m);
    tasks.erase(id);
}

int jobscheduler::Add(std::function<void()> task, std::time_t timestamp){
    std::lock_guard<std::mutex> lock(m);
    tasks[id] = {task, timestamp};
    cv.notify_all();
    return id++;
}