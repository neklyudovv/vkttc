## Стажировка в VK <br>Backend Developer (Профильное задание)**

**Задача**:  
Реализовать класс планировщик задач. В классе должен быть метод `Add(std::function<void()> task, std::time_t timestamp)`, параметры:  
- `task` — задача на выполнение,
- `timestamp` — время, не раньше которого задача должна быть выполнена.

Задание на знание потоков и примитивов синхронизации. Используем стандартную библиотеку.

---

## Содержание
- [Описание решения](#описание-решения)
- [Архитектура](#архитектура)
- [Установка и сборка](#установка-и-сборка)
- [Технические детали](#технические-детали)
- [TODO](#todo)

---

## Описание решения
TaskManager — это планировщик задач с использованием потоков. Задачи выполняются асинхронно в отдельном потоке, не блокируя основной поток программы. Это достигается благодаря применению `std::thread`, `std::mutex`, и `std::condition_variable`.

---

## Архитектура
1. **TaskManager**:
   - Хранит задачи в `unordered_map`, где ключом служит идентификатор задачи.
   - Использует поток для отслеживания времени выполнения задач.
   - Управляет синхронизацией с помощью мьютекса и условной переменной.

2. **Асинхронное выполнение**:
   - Задачи запускаются с использованием `std::thread` и `detach()` для обеспечения асинхронности.
   - Основной поток проверки задач не блокируется выполнением задач.

---

## Установка и сборка
1. Убедитесь, что у вас установлен CMake.
2. Склонируйте репозиторий:
   ```
   git clone https://github.com/neklyudovv/vkttc.git
   ```
3. Перейдите в директорию проекта:
   ```
   cd vkttc
   ```
4. Создайте папку сборки и соберите проект:
   ```
   mkdir build && cd build
   cmake ..
   make
   ```


---

## Технические детали
- **Потоковая модель**:
    - Основной поток `TaskManager` выполняет цикл с ожиданием появления новых задач и приостанавливает свою работу при их отсутствии.
    - При совпадении `timestamp` и текущего времени задача запускается в отдельном потоке.
- **Работа с задачами**
    - Добавление при помощи `Add(std::function task, std::time_t timestamp)`. Также автоматически возобновляет работу проверяющего потока, если требуется.
    - Удаление задачи публичным методом `eraseTask(int id)`
- **Синхронизация**:
    - Используется `std::mutex` для защиты контейнера с задачами, контроля доступа к методам `Add` и `eraseTask` и избежания гонки данных.
    - `std::condition_variable` используется для пробуждения потока при добавлении новой задачи.

---

## TODO
- Вместо `std::unordered_map` можно было бы использовать `std::map` для более упорядоченного хранения задач или сортировки по `timestamp`.
- Также оптимизировать работу можно при помощи `std::condition_variable::wait_until()`, чтобы поток засыпал до точного времени выполнения следующей задачи.
- Заняться обработкой ошибок, в том числе при вызове методов с неправильными аргументами (напр. уже прошедший `timestamp`).
- Провести нагрузочное тестирование для оценки производительности при большом количестве задач.