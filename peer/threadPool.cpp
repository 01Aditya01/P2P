#include "peer_header.h"

ThreadPool::ThreadPool(int numThreads) : running(true) {
    // Constructor implementation
    // Initialize worker threads and other private members
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this] {
                        return !running || !tasks.empty();
                    });
                    if (!running && tasks.empty()) {
                        return;
                    }
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

// void ThreadPool::Submit(std::function<void()> task) {
//     // Submit method implementation
//     // Enqueue tasks and notify worker threads
//     {
//         std::unique_lock<std::mutex> lock(queueMutex);
//         tasks.push(task);
//     }
//     condition.notify_one();
// }
bool ThreadPool::Submit(std::function<bool()> task) {
    // Submit method implementation
    // Enqueue tasks and notify worker threads
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task);
    }
    condition.notify_one();
}

ThreadPool::~ThreadPool() {
    // Destructor implementation
    // Set running to false, notify all threads, and join threads
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        running = false;
    }
    condition.notify_all();

    for (std::thread &worker : workers) {
        worker.join();
    }
}

// Additional private members and helper functions can be defined here
