#include<iostream>
#include<unordered_map>
#include<filesystem>
#include<fstream>
#include<vector>
#include<cctype>
#include<string>

namespace fs=std::filesystem;
using fstream=std::fstream;
std::string name;

class FileIndexer{
    private:
    fs::path root_path;
    std::unordered_map<fs::path,std::vector<fs::path>> index;

    public:
    void set_root(const std::string& root){
        if(root.empty()){
            std::cout<<"So your current directory is: "<<fs::path(".");
           root_path=fs::path(".");
        }
        else{
            std::cout<<"\n\nSo your root directory is: "<<root;
            root_path=root;
        }
    }

    void build_index() {
    index.clear();

    try {
        fs::recursive_directory_iterator it(
            root_path,
            fs::directory_options::skip_permission_denied
        );

        fs::recursive_directory_iterator end;

        while (it != end) {
            try {
                const fs::directory_entry& entry = *it;

                if (entry.is_regular_file()) {
                    fs::path filename = entry.path().filename();
                    fs::path filepath = entry.path();
                    index[filename].push_back(filepath);
                }
            }
            catch (const fs::filesystem_error&) {
                // Skip invalid entries
            }

            ++it;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Indexing failed: " << e.what() << '\n';
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
        std::string ans1;
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

class FileReader{
    std::string fileName;
    public:
    FileReader(std::string file_name) : fileName(file_name){
        check();
        std::ifstream file(fileName);
        read(file);
    }
    int check(){
        std::ifstream file(fileName);
        if(!file.is_open()){
            std::cout<<"\n\nFailed to open file\n\n\n";
            return 1;
        }
        return 0;
    }

    std::string read(std::ifstream &file){ //files are non-copyable resources, pass only reference, value not allowed
        std::string cursor;
        std::cout<<"\n\n";
        std::cout<<"The file says: \n\n";
        while(std::getline(file,cursor)){
            std::cout<<cursor<<std::endl;
        }
        std::cout<<"\n\n";    
        //file.close(); //files passed by reference aren't owned, so no need to close, doing anyways breaks caller's logic for borrowed file
        return "";
    }
};

int main(){
    std::cout<<"\033[2J\033[1;1H";
    
    bool search=false;
    bool open=false;
    Chat bot(search,open);

    if(bot.o){
        std::string file_name;
        std::cout<<"\n\nGot it "<<name<<"! which file do you want to open?\n\n\n";
        std::cin>>file_name;
        FileReader reader(file_name);
        //will continue later
    }
    else if(bot.s){
        
        FileIndexer indexer;

        std::cout<<"Enter the root directory to index from: (example- C:/users/)\t\t\t\t(or press Enter to index current directory)\n\n";
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