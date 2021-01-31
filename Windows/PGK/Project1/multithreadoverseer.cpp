#include "multithreadoverseer.h"

MultithreadOverseer::MultithreadOverseer(size_t workers_number) : number_of_workers_(workers_number)
{
    waiting_workers_ = 0;
    for(size_t i = 0; i < workers_number; ++i)
    {
        workers_.emplace_front(MultithreadOverseer::WorkLoop, this);
    }
}

MultithreadOverseer::~MultithreadOverseer()
{
    WaitForAllTasks();
    Stop();
}

void MultithreadOverseer::WorkLoop(MultithreadOverseer *context)
{
    std::unique_lock<std::mutex> wait_lock(context->waitMutex, std::defer_lock);
    std::future<void> function;
    while(!context->stop_threads_.load() || context->NumberOfTasks() > 0)
    {
        if(context->NumberOfTasks() > 0)
        {
            context->tasks_mutex_.lock();
            if(context->tasks_.size())
            {
                function = std::move(context->tasks_.front());
                context->tasks_.pop();
            }
            context->tasks_mutex_.unlock();
            if(function.valid())
            {
                function.wait();
            }
        }
        else
        {
            context->waiting_workers_++;
            wait_lock.lock();
            context->cv.wait(wait_lock, [&](){return (context->NumberOfTasks() > 0 || context->stop_threads_);});
            context->waiting_workers_--;
            wait_lock.unlock();
            context->cv.notify_all();
        }
    }
}

void MultithreadOverseer::AddTask(std::future<void> &&task)
{
    std::lock_guard<std::mutex> lock(tasks_mutex_);
    tasks_.push(std::move(task));
    cv.notify_one();
}

void MultithreadOverseer::Stop()
{
    stop_threads_ = true;
    cv.notify_one();
    while(waiting_workers_ != 0)
    {

    }
    for(auto &worker : workers_)
    {
            worker.join();
    }
}

void MultithreadOverseer::WaitForAllTasks()
{
    while(NumberOfTasks() && waiting_workers_ == number_of_workers_)
    {

    }
}

size_t MultithreadOverseer::NumberOfTasks()
{
    std::lock_guard<std::mutex> lock(tasks_mutex_);
    return tasks_.size();
}
