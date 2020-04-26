// Copyright (c) 2020 Emmanuel Arias
#include "include/worker_thread.h"

#include <cassert>
#include <iostream>
#include <optional>

#include "include/task.h"
#include "include/task_queue.h"

void WorkerThread(const int workerId, TaskQueue& taskQueue,
                  std::mutex& coutMutex) {
    auto& [m, cv] = taskQueue.Subscribe();

    while (true) {
        auto data = [&]() -> std::optional<Task*> {
            std::unique_lock l{m};
            cv.wait(l, [&] {
                return taskQueue.IsQueueStopped() || taskQueue.HasPendingTask();
            });
            if (taskQueue.IsQueueStopped()) {
                return {};
            } else {
                Task* taskToProcess = taskQueue.GetNextTask();
                assert(taskToProcess != nullptr);
                return taskToProcess;
            }
        }();

        if (!data) {
            break;
        }

        Task* taskPtr = *data;
        {
            std::scoped_lock<std::mutex> guard(coutMutex);
            std::cout << "Worker {" << workerId << "} is executing task {"
                      << taskPtr->GetId() << "}.\n";
        }

        // process the data
        taskPtr->Execute();
        delete taskPtr;
    }
}