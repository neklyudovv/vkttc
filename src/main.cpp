#include "../include/TaskManager.h"
#include <iostream>
#include <ctime>

void testTask() {
    std::cout << "test executed" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10)); // задержка, чтобы показать
}                                                          // асинхронное выполнение тасков

int main() {
    TaskManager manager;

    const std::time_t now = std::time(nullptr);

    // Добавление задач
    int firstTaskId = manager.Add(testTask, now + 3);
    manager.Add([] { std::cout << "task2 executed" << std::endl; }, now + 5);

    // Удаление первой задачи по id
    manager.eraseTask(firstTaskId);

    // Ожидание выполнения задач и появления новых
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Добавление новых задач, причем testTask содержит в себе sleep_for(10)
    manager.Add(testTask, now+15);
    manager.Add([] { std::cout << "task3 executed" << std::endl; }, now + 18);

    // Ожидание перед завершением программы
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}