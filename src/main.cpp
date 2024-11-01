#include "../include/TaskManager.h"
#include <iostream>
#include <thread>

void testTask() {
    std::cout << "test executed" << std::endl;
}

int main() {
    TaskManager manager;

    std::time_t now = std::time(nullptr);
    manager.Add(testTask, now + 3);
    manager.Add([] { std::cout << "task2 executed" << std::endl; }, now + 5);

    while(true) {
        manager.RunTasks();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}