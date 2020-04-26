// Copyright (c) 2020 Emmanuel Arias
#include <cassert>
#include <chrono>
#include <random>
#include <mutex>
#include <thread>
#include <vector>

#include "include/task.h"
#include "include/task_queue.h"
#include "include/worker_thread.h"


int main(int argc, char* argv[]) {
    const unsigned int thread_pool_size =
        std::thread::hardware_concurrency() - 1;
    assert(thread_pool_size > 0);

    std::mutex coutMutex;

    std::vector<std::thread> thread_pool(thread_pool_size);
    TaskQueue taskQueue;

    for (size_t i = 0; i < thread_pool_size; ++i) {
        thread_pool[i] = std::thread(WorkerThread, i, std::ref(taskQueue),
                                     std::ref(coutMutex));
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> taskDuration(0, 1000);

    for (int i = 0; i < 5; ++i) {
        taskQueue.PushTask(
            new Task(i, std::chrono::milliseconds(taskDuration(gen)),
                     std::ref(coutMutex)));
    }

    std::vector<Task*> taskBatch;
    taskBatch.resize(5);
    for (int i = 0; i < 5; ++i) {
        taskBatch[i] =
            new Task(i + 5, std::chrono::milliseconds(taskDuration(gen)),
                     std::ref(coutMutex));
    }
    taskQueue.PushTasks(taskBatch);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    taskQueue.StopQueue();

    for (size_t i = 0; i < thread_pool_size; ++i) {
        thread_pool[i].join();
    }
    return 0;
}
