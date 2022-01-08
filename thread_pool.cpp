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
                jobs_cv.wait(lock, [this](){
                    return !jobs_queue.empty() || terminate_pool.load(); 
                });

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

/*
template <class T>
auto ThreadPool::addJob(T job) -> std::future<decltype(job())>{
    auto wrapper = std::make_shared<std::packaged_task<decltype(job()) ()>>(std::move(job));    
    {
        unique_lock<mutex> lock(jobs_queue_mutex);
        //jobs_queue.push(job);
        
        jobs_queue.emplace([=]{
            (*wrapper)(); //jutro sprobowac z decltype, jak sie nie uda, poddac się
        });
        
    }
    jobs_cv.notify_one();
    return wrapper->get_future();
}
*/