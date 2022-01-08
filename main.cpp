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

//void showIncludedElements(const fs::path&);
void showIncludedElementsRecursively(const fs::path&);

mutex cout_mutex;
void printer(){
	{
		unique_lock<mutex> lock(cout_mutex);
		cout<<get_id()<<" Hello world!"<<endl;
		//debug
		for(unsigned i=0; i<(1<<12); i++){
			;
		}
	}
	
}

int return1(){
	{
		unique_lock<mutex> lock(cout_mutex);
		cout<<"Im returning 1"<<endl;
	}
	return 1;
}
int return2(){
	{
		unique_lock<mutex> lock(cout_mutex);
		cout<<"Im returning 2"<<endl;
	}
	return 2;
}

ThreadPool gtp(8);

void DisplayDirTree(const fs::path& pathToShow, int level = 0)
{
    if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
    {
        auto lead = std::string(level * 3, ' ');
        for (const auto& entry : fs::directory_iterator(pathToShow))
        {
            auto filename = entry.path().string();//.filename();
            if (fs::is_directory(entry.status()))
            {

                cout << lead << "[+] " << "DIR" << filename << "\n";
                DisplayDirTree(entry, level + 1);
                //cout << "\n";
            }
            else if (fs::is_regular_file(entry.status()))
                //DisplayFileInfo(entry, lead, filename);
				cout << lead << "[+] " << "FIL" << filename <<"\n";
				//cout << "\n";
			else
                cout << lead << "[?] " << filename << "\n";
        }
    }
}

fs::path global_path(".");

//dependent on cout_mutex
Statistics DisplayDirTreeUsingThreadPool(const fs::path& pathToShow, ThreadPool& tp, int level = 0){
	auto filename = pathToShow.string();
	if(fs::exists(pathToShow)){
		if(fs::is_regular_file(pathToShow)){
			//handle opening file
			Statistics result = Statistics{};
			result.files = 1;
			
			std::fstream file;
			file.open(filename, std::ios::in);
			
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
				cout << std::string(level * 3, ' ') << "[FIL] " << filename << endl;
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
				
				fs::path entry_path = entry.path();

				auto fut = tp.addJob(std::function<Statistics()>([&]{
					return DisplayDirTreeUsingThreadPool(entry_path, tp, level+1); //directory_entry i path to nie ten sam typ
				}));

				{
					unique_lock<mutex> lock(cout_mutex);
					//cout << std::string(level * 3, ' ') << "[FIL] " << filename << endl;
					cout<<entry.path().string().size()<<endl;
					cout<<entry.path().string()<<endl;
				}

				partial_results.emplace_back(std::move(fut));

			}

			for(int i=0;i<partial_results.size();i++){
				result += partial_results[i].get(); 
			}

			{
				unique_lock<mutex> lock(cout_mutex);
				cout << std::string(level * 3, ' ') << "[DIR] " << filename << endl;
				cout<<result<<endl;
			}
			return result;
		}
		else{ //another type of file-ish object
			Statistics no_result = Statistics{};
			{
				unique_lock<mutex> lock(cout_mutex);
				cout << std::string(level * 3, ' ') << "[???] " << filename << endl;
				cout<<no_result<<endl;
			}
			return no_result;
		}
	}
	else{
		Statistics no_result = Statistics{};
		{
				unique_lock<mutex> lock(cout_mutex);
				cout << std::string(level * 3, ' ') << "[ERR] " << filename << endl;
				cout<<no_result<<endl;
		}
		return no_result;
	}
	
}

int main(){
	//fs::path p1(".");
	
	//showIncludedElements(p1);
	//showIncludedElementsRecursively(p1);
	//cout<<thread::hardware_concurrency()<<endl;

	
	/*
	int sum = 0;
	//ThreadPool tp(8);
	
	std::vector<std::future<int>> partial_results;

	for(int i=0; i<10; i++){
		auto f = gtp.addJob(std::function<int()>([=](){
			{
				unique_lock<mutex> lock(cout_mutex);
				cout<<"Im returning: "<<i<<endl;
			}
			return i;
		}));
		partial_results.emplace_back(std::move(f));
		
	}

	for(int i=0;i<10;i++){
		sum += partial_results[i].get();
	}
	
	cout<<sum<<endl;
	*/
	

	//wiem, ze bez sensu, co traz napisalem, ale chce zobaczyc, jak to dziala

	//sleep_for(seconds(1)); //need to wait for all job to end

	/*
	Statistics st1 = Statistics{.files = 1, .non_empty_lines = 2, .empty_lines = 3, .words = 4, .letters = 5};
	cout<<st1<<endl;
	*/

	DisplayDirTreeUsingThreadPool("./test-dir/dir1", gtp);
	//ThreadPool tp(100);
	return 0;
}