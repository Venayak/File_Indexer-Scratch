#include<iostream>
#include<filesystem>
#include<unordered_map>
#include<vector>
#include<string>

namespace fs=std::filesystem;

int main(){
std::cout<<"\033[2J\033[1;1H";
//std::cout<<"Which directory?\n"; //USE \033[x;15H for 2 columns!!!

std::unordered_map<std::string,std::vector<std::string>> index;

for(const fs::directory_entry &entry : fs::recursive_directory_iterator(".", fs::directory_options::skip_permission_denied)){
    if(!entry.is_regular_file()){ //ignores folders, special files
        continue;
    }
    
    std::string filename=entry.path().filename().generic_string(); //string x="hello world";
    std::string filepath=entry.path().generic_string();

    index[filename].push_back(filepath); //inside index variable of type hashmap, corresponding value of key filename = it's corresponding filepath (index[key]=value; or index[key].push_back(value);)

    /*if(entry.is_directory()){
        std::cout<<"[DIR]";
    }
    else{                         
        std::cout<<"[FILE]";
    }
    std::cout<<entry.path().generic_string()<<"\n";
*/
}

for(const auto &[filename, paths] : index){
    std::cout<<filename<<"\n";
    for(const auto &p : paths){
        std::cout<<" "<<p<<"\n";
    }
    std::cout<<"\n\n";
} 

//for reference of [key, value] in index // value is not filepaths, we created a new variable &[a, b] (structured binding) that stores reference (&) to the key and the whole value vector containing possibly multiple filepaths

/*std::string key="main.cpp";
auto it=index.find(key); //map.find(key) is used to search for the pair<key, value> 
if(it!=index.end()){     //if the search didn't end, i.e. entry exists,
    const std::vector<std::string> &paths=it->second; //
    for(const auto &p : paths){
        std::cout<<p<<"\n";
    }
}*/
return 0;    
}