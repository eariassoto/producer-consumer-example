// Copyright (c) 2020 Emmanuel Arias
#include "include/task.h"

#include <iostream>
#include <thread>

Task::Task(unsigned int id, std::chrono::milliseconds duration,
           std::mutex& coutMutex)
    : m_Id{id}, m_Duration{duration}, m_CoutMutex{coutMutex} {}

void Task::Execute() {
    std::this_thread::sleep_for(m_Duration);
    {
        std::scoped_lock<std::mutex> guard(m_CoutMutex);
        std::cout << "Task {" << m_Id << "} finished in " << m_Duration.count()
                  << "ms.\n";
    }
}
