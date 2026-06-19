#include<iostream>
#include<string>
#include<filesystem>
#include<set>
#include<fstream>
#include<print>

using namespace std;

namespace fs = filesystem;

// This is a function for opening the files in the filestream and to mark exceptions for all.
// This is done, because, there is no inbuilt function to mark all the filestreams with exceptions.
ifstream open_file(string &path){
	ifstream filestream(path);
	filestream.exceptions(
		ifstream :: failbit | ifstream :: badbit
	);
	return filestream;
}

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
	println("{:<8} {:<20} {}", "PID", "NAME", "COMMAND");
	for(auto i : ordered_pids){
		string name_path = "/proc/" + to_string(i) + "/comm";
		string command_path = "/proc/" + to_string(i) + "/cmdline";
		auto namestream = open_file(name_path);
		auto cmd_stream = open_file(command_path);
		if(!namestream or !cmd_stream) continue;
		try {
			string name, command;
			getline(namestream, name);
			getline(cmd_stream, command);
			println("{:<8} {:<20} {}", i, name, command.substr(0, 50)); 
		}
		catch(...){
			// The catch block has to be kept silent, according to the character of ps.
			// I'm defining it under the try block, as, there is a possibility that, the process may become dead...
		}
	}
} 