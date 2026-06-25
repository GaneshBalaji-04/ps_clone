#include<iostream>
#include<string>
#include<filesystem>
#include<set>
#include<fstream>
#include<print>
#include<vector>
#include<ranges>
#include<string_view>
#include<sys/stat.h>
#include<sys/sysmacros.h>

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
	println("{:<8} {:<20} {:<60} {}", "PID", "NAME", "COMMAND", "TTY");
	for(auto i : ordered_pids){
		string name_path = "/proc/" + to_string(i) + "/comm";
		string command_path = "/proc/" + to_string(i) + "/cmdline";
		string tty_path = "/proc/" + to_string(i) + "/stat";
		ifstream name_stream(name_path);
		set_exceptions(name_stream);
		ifstream cmd_stream(command_path, ios::binary);
		set_exceptions(cmd_stream);
		ifstream tty_stream(tty_path);
		set_exceptions(tty_stream);
		if(!name_stream or !cmd_stream or !tty_stream) continue;
		
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
			string whole_stat;
			getline(tty_stream, whole_stat);
			auto segments = whole_stat | ranges::views::split(' ');
			int tty_temp = 1;
			string tty_value;
			for(const auto &word : segments){
				if(tty_temp == 7) break;
				tty_value = string(word.begin(), word.end());
				if(tty_value[0] == '(' or tty_value[tty_value.size()-1] == ')') tty_temp = 2;
				else tty_temp++;
			}
			int tty_mid = stoi(tty_value);
			unsigned int major_num = major(tty_mid);
			unsigned int minor_num = minor(tty_mid);
			dev_t tty = makedev(major_num, minor_num);
			string device;
			bool sign = false;
			for(auto entry : fs::recursive_directory_iterator("/dev")){
				struct stat sb;
				if(fs::is_character_file(entry.status())) {
					if(stat(entry.path().c_str(), &sb) == 0) {
						if(sb.st_rdev == tty){
							device = entry.path().string();
							sign = true;
							break;
						}
						if(sign) break;
					}
					if(sign) break;
				}
				if(sign) break;
			}
			if(sign == false){
				println("{:<8} {:<20} {:<60} ?", i, name, command.substr(0, 50)); 
			}
			else{
				println("{:<8} {:<20} {:<60} {}", i, name, command.substr(0, 50), device); 
			}
		}
		catch(...) {
    			
		}
	}
} 