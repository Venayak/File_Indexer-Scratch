#include<iostream>
#include<filesystem>
#include<unordered_map>
#include<vector>

namespace fs=std::filesystem;

int main(){
std::cout<<"\033[2J\033[1;1H";
//std::cout<<"Which directory?\n"; //USE \033[x;15H for 2 columns!!!

std::unordered_map<std::string,std::vector<std::string>> index;

for(const fs::directory_entry &entry: fs::recursive_directory_iterator(".")){
    if(!entry.is_regular_file()){ //ignores folders, special files
        continue;
    }
    
    std::string filename=entry.path().filename().generic_string(); //string x="hello world";
    std::string filepath=entry.path().generic_string();

    index[filename].push_back(filepath); //inside index variable of type hashmap, corresponding value of key filename = it's corresponding filepath (index[key]=value; or index[key].push_back(value);)

    if(entry.is_directory()){
        std::cout<<"[DIR]";
    }
    else{                         
        std::cout<<"[FILE]";
    }
    std::cout<<entry.path().generic_string()<<"\n";
}

return 0;    
}