// Copyright (c) 2020 Emmanuel Arias
#pragma once
#include <mutex>
#include <queue>
#include <tuple>

class Task;

class TaskQueue {
   public:
    ~TaskQueue();

    // Thread safe functions
    void PushTask(Task* t);
    void PushTasks(std::vector<Task*>& tasks);
    void StopQueue();
    // Way for consumers to get the sync variables
    std::tuple<std::mutex&, std::condition_variable&> Subscribe();

    // Non-thread safe function. Consumers must ensure
    // lock acquisition
    bool HasPendingTask() const { return !m_Queue.empty(); }
    bool IsQueueStopped() const { return m_QueueIsStopped; }
    Task* GetNextTask();

   private:
    bool m_QueueIsStopped = false;
    std::queue<Task*> m_Queue;
    std::mutex m_Mutex;
    std::condition_variable m_ConditionVariable;
};