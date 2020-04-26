// Copyright (c) 2020 Emmanuel Arias
#pragma once
#include <chrono>
#include <mutex>

class Task {
   public:
    Task(unsigned int id, std::chrono::milliseconds duration,
         std::mutex& coutMutex);

    void Execute();

    unsigned int GetId() const { return m_Id; }

   private:
    unsigned int m_Id = 0;
    std::chrono::milliseconds m_Duration;
    std::mutex& m_CoutMutex;
};
