#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <vector>
#include <queue>
#include <future>


using std::thread;
using std::mutex;
using std::condition_variable;
using std::atomic;
using std::function;
using std::vector;
using std::queue;
using std::future;
using std::unique_lock;

using Task = std::function<void()>;


class ThreadPool{
    queue<Task> jobs_queue;
    mutex jobs_queue_mutex;
    condition_variable jobs_cv;
    vector<thread> pool;
    atomic<bool> terminate_pool;
    atomic<int> waitingThreads;

    void KeepWaitingForJobs();

    public:
    ThreadPool(int num_threads);
    ~ThreadPool();
    int getWaitingThreads();
    void addSingleThread();

    //debug
    int getNumberOfThreads(){
        return pool.size();
    }

    template <class T>
    auto addJob(T job) -> std::future<decltype(job())>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(job()) ()>>(std::move(job));    
        {
            unique_lock<mutex> lock(jobs_queue_mutex);
            
            jobs_queue.emplace([=]{
                (*wrapper)();
            });
            
        }
        jobs_cv.notify_one();
        return wrapper->get_future();
    }
    
};

#endif