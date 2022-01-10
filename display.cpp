#include "display.hpp"

#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <fstream>
#include "thread_pool.hpp"
#include "staticstics.hpp"

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

Statistics DisplayDirTreeUsingThreadPool(const std::string& pathName, ThreadPool& tp){
	fs::path pathToShow(pathName);
	if(fs::exists(pathToShow)){
		if(fs::is_regular_file(pathToShow)){
			//handle opening file
			Statistics result = Statistics{};
			result.files = 1;
			
			std::fstream file;
			file.open(pathName, std::ios::in);
			
			std::string line;
			while(getline(file, line)){
				if(line.empty()){
					result.empty_lines++;
				}
				else{
					result.non_empty_lines++;
					for(int i=0;i<line.size();i++){
						if((unsigned)line[i]>32 && (unsigned)line[i]<127){
							result.letters++;
							if(i==0 || line[i-1]==' ' || line[i-1]=='\t'){
								result.words++;
							}
						}
					}
				}
			}

			file.close();

			{
				unique_lock<mutex> lock(cout_mutex);
				cout << "[FIL] " << pathName << endl;
				cout<<result<<endl;
			}
		
			return result;
		}
		else if(fs::is_directory(pathToShow)){
			

			//handle opening directory
			Statistics result{};
			//vector of futures
			vector<future<Statistics>> partial_results;
			for(const auto& entry : fs::directory_iterator(pathToShow)){
				
				std::string entry_path_name = entry.path().string();

				auto fut = tp.addJob(std::function<Statistics()>([=, &tp]{
					return DisplayDirTreeUsingThreadPool(entry_path_name, tp);
				}));

				partial_results.emplace_back(std::move(fut));

				
				if(tp.getWaitingThreads() == 0){
					tp.addSingleThread();
				}
				

			}
			
            if(tp.getWaitingThreads() == 0){
					tp.addSingleThread();
			}

			for(int i=0;i<partial_results.size();i++){
				result += partial_results[i].get(); 
			}

			{
				unique_lock<mutex> lock(cout_mutex);
				cout << "[DIR] " << pathName << endl;
				cout<<result<<endl;
			}
			return result;
		}
		else{ //another type of file-ish object
			Statistics no_result = Statistics{};
			{
				unique_lock<mutex> lock(cout_mutex);
				cout << "[???] " << pathName << endl;
				cout<<no_result<<endl;
			}
			return no_result;
		}
	}
	else{
		Statistics no_result = Statistics{};
		{
				unique_lock<mutex> lock(cout_mutex);
				cout << "[ERR] " << pathName << endl;
				cout<<no_result<<endl;
		}
		return no_result;
	}
	
}