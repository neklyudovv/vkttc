## JobScheduler

**JobScheduler** is a lightweight scheduled job runner built on top of threads.
Jobs are executed asynchronously in a separate worker thread, without blocking the main program flow.
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

The project implements a class for scheduling and executing jobs at predefined timestamps using a dedicated worker thread and synchronization primitives.
Internally, jobs are represented as tasks stored in a container.

Key methods: `Add`, `eraseTask`, `startThread`.

### `Add`

* **Parameters**: `std::function<void()> task`, `std::time_t timestamp`
* **Description**: Inserts a new task (job) into the container and wakes the worker thread if needed.

### `eraseTask`

* **Parameters**: `int id`
* **Description**: Removes a task by its identifier.

### `startThread` (internal)

* **Description**: Starts the worker thread.
  The thread monitors pending tasks and executes the corresponding job when its timestamp is reached.
  If no tasks are available, the thread waits until it receives a notification.

---

## Architecture

1. **JobScheduler**

    * Stores scheduled tasks in an `unordered_map` keyed by job ID.
    * Uses a dedicated worker thread for scheduling.
    * Ensures thread-safety with a mutex and condition variable.

2. **Asynchronous Execution**

    * Each job is executed asynchronously using `std::thread` with `detach()`.
    * The scheduler thread never blocks while a job is running.

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