#include<iostream>
#include<string>
#include<filesystem>
#include<set>

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
        cout<<"PIDs"<<endl<<endl;
        for(auto path : ordered_pids){
                cout<<path<<endl;
        }
}