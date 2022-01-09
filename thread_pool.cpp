#include "thread_pool.hpp"
#include <future>
#include <functional>

using std::unique_lock;
using std::move;
using std::future;


void ThreadPool::KeepWaitingForJobs(){
    while(true){
        {
            Task job;
            {
                unique_lock<mutex> lock(jobs_queue_mutex);
                
                if(jobs_queue.empty()){
                    waitingThreads++;
                    jobs_cv.wait(lock, [this](){
                        return !jobs_queue.empty() || terminate_pool.load(); 
                    });
                    waitingThreads--;
                }
                
                if(jobs_queue.empty() && terminate_pool.load()){
                    break;
                }            

                job = move(jobs_queue.front());
                jobs_queue.pop();
            }
            job();
        }
    }
}

ThreadPool::ThreadPool(int num_threads = thread::hardware_concurrency()){
    waitingThreads = 0;
    terminate_pool = false;
    for(int i = 0; i < num_threads; ++i){
        pool.push_back(thread(&ThreadPool::KeepWaitingForJobs, this));
    }
}

ThreadPool::~ThreadPool(){
    terminate_pool = true;
    jobs_cv.notify_all();
    for(thread &th : pool){
        th.join();
    }
    pool.clear();
}

int ThreadPool::getWaitingThreads(){
    return waitingThreads.load();
}

void ThreadPool::addSingleThread(){
    pool.emplace_back(thread(&ThreadPool::KeepWaitingForJobs, this));
}
