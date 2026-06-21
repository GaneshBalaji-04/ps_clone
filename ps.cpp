#include<iostream>
#include<string>
#include<filesystem>
#include<set>
#include<fstream>
#include<print>
#include<vector>

using namespace std;

namespace fs = filesystem;

// This is a function for opening the files in the filestream and to mark exceptions for all.
// This is done, because, there is no inbuilt function to mark all the filestreams with exceptions.
void set_exceptions(ifstream &filestream){
	filestream.exceptions(
		ifstream :: badbit
	);
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
		ifstream name_stream(name_path);
		set_exceptions(name_stream);
		ifstream cmd_stream(command_path, ios::binary);
		set_exceptions(cmd_stream);
		if(!name_stream or !cmd_stream) continue;
		
		try {
			string name;
			getline(name_stream, name);
			vector<string> arguments;
			string temp_command;
			while(getline(cmd_stream, temp_command, '\0')){
				arguments.push_back(temp_command);
			}	
			string command;
			for(string &s: arguments){
				command.append(s);
				command.push_back(' ');
			}		
			println("{:<8} {:<20} {}", i, name, command.substr(0, 50)); 
		}
		catch(...) {
    			
		}
	}
} 