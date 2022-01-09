#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <fstream>
#include "thread_pool.hpp"
#include "staticstics.hpp"
#include "display.hpp"

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

int main(int argc, char *argv[]){
	ThreadPool gtp(4);

	DisplayDirTreeUsingThreadPool("./test-dir/dir1/outoftouch.txt", gtp);
	cout<<gtp.getNumberOfThreads()<<endl;
	return 0;
}