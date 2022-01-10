#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <fstream>
#include "thread_pool.hpp"
#include "staticstics.hpp"
#include "display.hpp"
#include "timer.hpp"

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
using std::fstream;

const int num_threads = 1;

int main(int argc, char *argv[]){
	ThreadPool gtp(num_threads);
	Timer timer;
	timer.start();
	DisplayDirTreeUsingThreadPool("./test-dir", gtp);
	timer.stop();
	cout<<"Benchmark results:"<<endl;
	cout<<"Number of threads in the beginning: "<<num_threads<<endl;
	cout<<"Number of threads in the end: "<<gtp.getNumberOfThreads()<<endl;
	cout<<"Execution time: "<<timer.getTime()<<"s."<<endl;
	return 0;
}