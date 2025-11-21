## TaskManager

**TaskManager** is a lightweight scheduled functions runner based on threads.
Tasks are executed asynchronously in a separate worker thread, without blocking the main program flow.
Synchronization is handled via `std::thread`, `std::mutex`, and `std::condition_variable`.

---

## Contents

* [Solution Overview](#solution-overview)
* [Architecture](#architecture)
* [Build & Installation](#build--installation)
* [Technical Details](#technical-details)
* [TODO](#todo)

---

## Solution Overview

The project implements a class for managing and executing timed tasks using a dedicated thread and synchronization primitives.
Key methods: `Add`, `eraseTask`, `startThread`.

### `Add`

* **Parameters**: `std::function<void()> task`, `std::time_t timestamp`
* **Description**: Inserts a task into the container and wakes the worker thread if it’s waiting.

### `eraseTask`

* **Parameters**: `int id`
* **Description**: Removes a task by its identifier.

### `startThread` (internal)

* **Description**: Launches the worker thread.
  The thread monitors pending tasks and executes them at the specified time.
  If no tasks exist, it waits until notified.

---

## Architecture

1. **TaskManager**

    * Stores tasks in an `unordered_map` keyed by task ID.
    * Uses a dedicated worker thread for scheduling.
    * Ensures thread-safety with a mutex and condition variable.

2. **Asynchronous Execution**

    * Tasks run asynchronously using `std::thread` with `detach()`.
    * The scheduler thread never blocks on task execution.

---

## Build & Installation

1. Ensure CMake is installed.
2. Clone the repository:

   ```
   git clone https://github.com/neklyudovv/vkttc.git
   ```
3. Enter the project directory:

   ```
   cd vkttc
   ```
4. Create a build directory and compile:

   ```
   mkdir build && cd build
   cmake ..
   make
   ```

---

## Technical Details

* **Thread model**

    * The worker thread waits for incoming tasks and sleeps when none are available.
    * When the current time reaches the task’s timestamp, the task is executed in a new thread.

* **Synchronization**

    * `std::mutex` protects the task container and all operations modifying it.
    * `std::condition_variable` is used to wake the worker thread on updates.

---

## TODO

* Improve scheduling with `std::condition_variable::wait_until()`.
* Add error handling (e.g., invalid timestamps such as values in the past).
* Run load testing to evaluate performance with a large number of tasks.