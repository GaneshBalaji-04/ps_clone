#include<iostream>
#include<string>
#include<filesystem>
#include<set>
#include<fstream>
#include<print>

using namespace std;

namespace fs = filesystem;

int main(){
	string path = "/proc";
	set<int> ordered_pids;
	for(const auto &entry : fs::directory_iterator(path)){
		string entry_path = entry.path().filename();
		string temporary_path;
		for(int i=0; i<entry_path.size(); i++){
			if(isdigit(entry_path[i])){
				temporary_path.push_back(entry_path[i]);
			}
			else break;
		}
		if(temporary_path.size() != 0){
			ordered_pids.insert(stoi(temporary_path));
		}
	}
	println("{:<8} Name\n", "PID");
	for(auto i : ordered_pids){
		string result_path = "/proc/" + to_string(i) + "/comm";
		ifstream filestream(result_path);
		filestream.exceptions(
			ifstream :: failbit | ifstream :: badbit
		);
		if(!filestream) continue;
		try {
			string line;
			getline(filestream, line);
			println("{:<8} {}", i, line); 
		}
		catch(...){
			// The catch block has to be kept silent, according to the character of ps.
			// I'm defining it under the try block, as, there is a possibility that, the process may become dead...
		}
	}
} 