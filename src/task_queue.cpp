// Copyright (c) 2020 Emmanuel Arias
#include "include/task_queue.h"

#include <iostream>
#include <thread>

#include "include/task.h"

TaskQueue::~TaskQueue() {
    while (!m_Queue.empty()) {
        Task* task = m_Queue.front();
        delete task;
        m_Queue.pop();
    }
}

std::tuple<std::mutex&, std::condition_variable&>
TaskQueue::Subscribe() {
    return std::make_tuple(std::ref(m_Mutex), std::ref(m_ConditionVariable));
}

void TaskQueue::PushTask(Task* t) {
    {
        std::scoped_lock l{m_Mutex};
        m_Queue.push(t);
    }
    m_ConditionVariable.notify_one();
}

void TaskQueue::PushTasks(std::vector<Task*>& tasks) {
    {
        std::scoped_lock l{m_Mutex};
        for (Task* t : tasks) {
            m_Queue.push(t);
        }
    }
    m_ConditionVariable.notify_all();
}

Task* TaskQueue::GetNextTask() {
    Task* taskPtr = m_Queue.front();
    m_Queue.pop();

    return taskPtr;
}

void TaskQueue::StopQueue() {
    std::unique_lock l{m_Mutex};
    m_QueueIsStopped = true;
    m_ConditionVariable.notify_all();
}
