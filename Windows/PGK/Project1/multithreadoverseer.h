#ifndef MULTITHREADOVERSEER_H
#define MULTITHREADOVERSEER_H
#include <atomic>
#include <condition_variable>
#include <forward_list>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

#include "particledatacontainer.h"

class MultithreadOverseer
{
public:
    explicit MultithreadOverseer(size_t workers_number);
    ~MultithreadOverseer();
    MultithreadOverseer(MultithreadOverseer &object) = delete;

    void AddTask(std::future<void> &&task);
    void Stop();
    void WaitForAllTasks();
    size_t NumberOfTasks();
    const size_t number_of_workers_;
private:
    std::forward_list<std::thread> workers_;
    std::mutex tasks_mutex_;
    std::mutex waitMutex;
    std::atomic<bool> stop_threads_{false};
    std::atomic<size_t> waiting_workers_;
    std::condition_variable cv;
    std::queue<std::future<void>> tasks_;
    static void WorkLoop(MultithreadOverseer *parentThread);
};

#endif // MULTITHREADOVERSEER_H
