#include<iostream>
#include<string>
#include<filesystem>

namespace fs = std::filesystem;

using namespace std;

int main(){
    string path = "/proc";
    cout<<"PID"<<endl<<endl;
    for(const auto &entry : fs::directory_iterator(path)){
        string entry_path = entry.path();
        string temporary_path;
        for(int i=6; i<entry_path.size(); i++){
                int ascii_value_of_character = (int)entry_path[i];
                if(ascii_value_of_character >= 48 and ascii_value_of_character <= 57){
                        temporary_path += entry_path[i];
                }
                else
                        break;
        }
        if(temporary_path.size() != 0){
                cout<<temporary_path<<endl;
        }
    }
}