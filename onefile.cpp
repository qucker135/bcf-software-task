#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <mutex>
//#include "thread_pool.hpp"
#include <condition_variable>
#include <atomic>
#include <functional>
#include <vector>
#include <queue>
#include <future>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::this_thread::sleep_for;
using std::this_thread::get_id;
using std::unique_lock;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::queue;
using std::mutex;
using std::condition_variable;
using std::atomic;
using std::function;
using std::vector;
using std::queue;
using std::future;

using Task = std::function<void()>;

class ThreadPool{
    queue<Task> jobs_queue;
    mutex jobs_queue_mutex;
    condition_variable jobs_cv;
    vector<thread> pool;
    atomic<bool> terminate_pool;

    void KeepWaitingForJobs(){
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

    public:
    ThreadPool(int num_threads = thread::hardware_concurrency()){
        terminate_pool = false;
        for(int i = 0; i < num_threads; ++i){
            pool.push_back(thread(&ThreadPool::KeepWaitingForJobs, this));
        }
    }
    ~ThreadPool(){
        terminate_pool = true;
        jobs_cv.notify_all();
        for(thread &th : pool){
            th.join();
        }
        pool.clear();
    }

    template <typename T>
    auto addJob(T job) -> std::future<decltype(job())>{
        auto wrapper = std::make_shared<std::packaged_task<int ()>>(std::move(job));    
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
};

int main(){
    ThreadPool tp(10);
    auto f1 = tp.addJob([]{
        return 997;
    });
    cout<<f1.get()<<endl;
    return 0;
}