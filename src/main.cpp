#include "../include/TaskManager.h"
#include <iostream>
#include <ctime>

void testTask() {
    std::cout << "test executed" << std::endl;
}

int main() {
    TaskManager manager;

    const std::time_t now = std::time(nullptr);
    manager.Add(testTask, now + 3);
    manager.Add([] { std::cout << "task2 executed" << std::endl; }, now + 5);
    //std::cout << "sync test" << std::endl;
    return 0;
}