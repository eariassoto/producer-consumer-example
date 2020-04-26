// Copyright (c) 2020 Emmanuel Arias
#pragma once
#include <mutex>

class TaskQueue;

void WorkerThread(const int workerId, TaskQueue& taskQueue,
                  std::mutex& coutMutex);