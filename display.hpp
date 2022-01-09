#ifndef __DISPLAY_HERE_HPP__
#define __DISPLAY_HERE_HPP__

#include "staticstics.hpp"
#include "thread_pool.hpp"
#include <mutex>
using std::mutex;

inline mutex cout_mutex;
Statistics DisplayDirTreeUsingThreadPool(const std::string& pathName, ThreadPool& tp);

#endif