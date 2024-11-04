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

/**
* Запускает поток для выполнения задач в нужное время
*
* Этот метод создаст поток, который будет постоянно проверять наличие задач, и
* если timestamp совпадает с текущим временем, задача будет запущена в отдельном потоке.
*
* Использует условную переменную для ожидания новых задач, мьютекс для обеспечения
* безопасного доступа к данным из нескольких потоков.
*/
void TaskManager::startThread(){
    taskThread = std::thread([this](){ // анонимная лямбда функция для обращения к tasks
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]() {return !tasks.empty() || !running;});
        if (!running && tasks.empty()) return;

        std::time_t now = std::time(nullptr);
        for (auto it = tasks.begin(); it != tasks.end();)
            if (it->second.timestamp <= now) {
                std::thread currTask = std::thread([func=it->second.func](){func();});
                currTask.detach();
                // запуск task в новом потоке, чтобы не дожидаться выполнения функции
                // ресурсы освободятся после завершения потока автоматически, но из-за этого
                // лучше не использовать с функциями, которые надо строго контроллировать
                it = tasks.erase(it); // передаем в итератор следующий элемент, чтобы он не указывал на уже удаленный
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
    // убрал блок {}, чтобы при return id++ не возникало конфликтов с одновременной инкрементацией
    std::lock_guard<std::mutex> lock(m);
    tasks[id] = {task, timestamp};
    cv.notify_all();
    return id++;
}