#include<iostream>
#include<filesystem>
#include<unordered_map>
#include<string>
#include<vector>

namespace fs=std::filesystem;

class FileIndexer{
    private:
    fs::path root_path;
    std::unordered_map<fs::path,std::vector<fs::path>> index;

    public:
    void set_root(const std::string& root){
        if(root.empty()){
            std::cout<<"So your root directory is: "<<fs::path(".");
           root_path=fs::path(".");
        }
        else{
            std::cout<<"\n\nSo your root directory is: "<<root;
            root_path=root;
        }
    }

    void build_index(){
        index.clear();
        fs::path filename,filepath;

        for(const fs::directory_entry &entry : fs::recursive_directory_iterator(root_path, fs::directory_options::skip_permission_denied)){
            if(!entry.is_regular_file()){
                continue;
            }
            filename=entry.path().filename();
            filepath=entry.path();
            index[filename].push_back(filepath);
        }
    }

    void print_index(){
        for(const auto &[file, paths] : index){
            std::cout<<"\n\n\n[DIR]-> "<<file.generic_string()<<"\n  |\n";
            for(const auto &p : paths){
                std::cout<<"  |----"<<p.generic_string()<<"\n";
            }
        }
    }
};

int main(){
    std::cout<<"\033[2J\033[1;1H";

    FileIndexer indexer;

    std::cout<<"Enter the root directory to index from: (example- C:/users/)\n\n";
    std::string root;
    std::getline(std::cin,root);

    indexer.set_root(root);
    indexer.build_index();
    indexer.print_index();
}