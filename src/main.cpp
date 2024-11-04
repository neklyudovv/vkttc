#include "../include/TaskManager.h"
#include <iostream>
#include <ctime>

void testTask() {
    std::cout << "test executed" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

int main() {
    TaskManager manager;

    const std::time_t now = std::time(nullptr);

    int firstTaskId = manager.Add(testTask, now + 3);
    manager.Add([] { std::cout << "task2 executed" << std::endl; }, now + 5);

    manager.eraseTask(firstTaskId);
    std::this_thread::sleep_for(std::chrono::seconds(10));

    manager.Add(testTask, now+15);
    manager.Add([] { std::cout << "task3 executed" << std::endl; }, now + 18);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}