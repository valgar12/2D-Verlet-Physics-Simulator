#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false), tasksRemaining(0) {
    for (size_t i = 0; i < numThreads; i++) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this]() {
                        return this->stop || !this->tasks.empty();
                        });
                    if (this->stop && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
                if (--tasksRemaining == 0) {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    finishedCondition.notify_one();
                }
            }
            });
    }
}

void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task);
        tasksRemaining++;
    }
    condition.notify_one();
}

void ThreadPool::waitAll() {
    std::unique_lock<std::mutex> lock(queueMutex);
    finishedCondition.wait(lock, [this]() {
        return tasksRemaining.load() == 0 && tasks.empty();
        });
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
        worker.join();
    }
}