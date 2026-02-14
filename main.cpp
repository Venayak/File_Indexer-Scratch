#include<iostream>
#include<filesystem>
#include<unordered_map>
#include<string>
#include<vector>
#include<cctype>

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

class Chat{
    public:
    bool s,o;
    Chat(bool search, bool open) : s(search), o(open){
        std::cout<<"Hey! I'm Ven,\nWhat is your name?\n\n\n";
        std::string name,ans1;
        std::cin>>name;
        std::cout<<"\n\nHey "<<name<<"! How can I assist you today?\n\n\n";
        //while(1)
        std::cin.ignore();
        while(ans1.empty()){
            std::getline(std::cin,ans1);
            if(ans1.empty()){
                std::cout<<"\n\nHmm?\n\n\n";
            }
            else{
                std::string word="";
                for(const char c : ans1){
                    word+=tolower(c);
                    if(word=="open"){
                            o=true;
                            break;
                    }
                    if(word=="search"){
                            s=true;
                            break;
                        }
                    if(c==' '){
                        if(word=="open"){
                            o=true;
                            break;
                        }
                        else if(word=="search"){
                            s=true;
                            break;
                        }
                        word="";
                    }
                }
            }        
        }
    }
};

int main(){
    std::cout<<"\033[2J\033[1;1H";
    
    bool search=false;
    bool open=false;
    Chat bot(search,open);

    if(bot.o){
        std::cout<<"\n\nreceived open\n\n\n";
    }
    else if(bot.s){
        std::cout<<"\n\nreceived search\n\n\n";
        
        FileIndexer indexer;

        std::cout<<"Enter the root directory to index from: (example- C:/users/)\n\n";
        std::string root;
        std::getline(std::cin,root);

        indexer.set_root(root);
        indexer.build_index();
        indexer.print_index();
    }
    
    std::cin.ignore();
    std::cout<<"\n\n\n******************************************************************************PRESS ENTER TO EXIT*****************************************************************************\n";
    std::cin.get();
}