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

class Chat{
    public:
    bool s,o;
    Chat(bool search, bool open) : s(search), o(open){
        std::string ans1;
        
        std::cout<<"\n\nHey "<<name<<"! How can I assist you today?\n\n\n";
        //while(1)
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

class FileIndexer{
    private:
    fs::path root_path;
    std::unordered_map<fs::path,std::vector<fs::path>> index;
    std::vector<fs::path> flat_results;

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

    void build_index(){
    index.clear();
    flat_results.clear();

    try {
        fs::recursive_directory_iterator it(
            root_path,
            fs::directory_options::skip_permission_denied
        );

        fs::recursive_directory_iterator end;

        while (it !=end){
            try {
                const fs::directory_entry& entry = *it;

                if (entry.is_regular_file()){
                    fs::path filename = entry.path().filename();
                    fs::path filepath = entry.path();
                    index[filename].push_back(filepath);
                    flat_results.push_back(filepath);
                }
            }
            catch (const fs::filesystem_error&){
                //to skip invalid entries
            }

            ++it;
        }
    }
    catch (const fs::filesystem_error& e){
        std::cerr<<"Indexing failed: "<<e.what()<<"\n\n";
        return;
    }
}

    void print_index(){
        int i = 1;
        for(const auto &[file, paths] : index){
            std::cout<<"\n\n\n[DIR]-> "<<file.generic_string()<<"\n  |\n";
            for(const auto &p : paths){
                std::cout<<"  |----["<<i<<"] "<<p.generic_string()<<"\n";
                i++;
            }
        }
    }

    const std::vector<fs::path>& get_results() const{
        return flat_results;
    }
};

class FileReader{
    std::string fileName;
    public:
    FileReader(std::string file_name) : fileName(file_name){
        check();
    }

    int check(){
        fs::path p(fileName);

        std::string ext = p.extension().string();
        for(auto &c : ext){
            c=tolower(c);
        }

        if(ext == ".docx" || ext == ".pdf"){
            std::cout << "\n\nConverting to .txt...\n\n";

            std::string command = ".\\Preprocessor_Util_Script.exe \"" + fileName + "\""; // \" is yet another escape sequence(since starting with \)

            int result = system(command.c_str());

            if(result != 0){
                std::cout << "\n\nFailed to convert file\n\n\n";
                    return 1;
            }

            std::cout << "\n\nConversion complete\n\n\n";

            p.replace_extension(".txt");

            std::ifstream file(p.string());

            if(!file.is_open()){
                std::cout << "\n\nFailed to open converted .txt file\n\n\n";
                return 1;
            }

            read(file);
        }

        else{
            std::ifstream file(fileName);

            if(!file.is_open()){
                std::cout << "\n\nFailed to open file\n\n\n";
                return 1;
            }

            read(file);
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
        return "***********************************************************************************END OF FILE*********************************************************************************\n";
    }
};


int main(){
    std::cout<<"\033[2J\033[1;1H";
    
    
    std::cout<<"Hey! I'm Ven,\nWhat is your name?\n\n\n";
    std::cin>>name;
    std::cin.ignore();
    
    while(1){
        bool search=false;
        bool open=false;
        Chat bot(search,open);

        if(bot.o){
            std::string file_name;
            std::cout<<"\n\nGot it "<<name<<"! which file do you want to open?\n\n\n";
            std::getline(std::cin, file_name);
            FileReader reader(file_name);
            //will continue later
        }
        else if(bot.s){
            
            FileIndexer indexer;

            std::cout<<"\n\nEnter the root directory to index from: (example- C:/users/)\t\t\t\t(or press Enter to index current directory)\n\n";
            std::string root;
            std::getline(std::cin,root);

            indexer.set_root(root);
            indexer.build_index();
            indexer.print_index();

            const auto& results = indexer.get_results();

            if(results.empty()){
                std::cout<<"\n\nNo files found.\n";
            }
            else{
                int choice;
                std::cout<<"\n\nEnter file number to open: ";
                std::cin>>choice;
                std::cin.ignore();

                if(choice>= 1&& choice<=results.size()){
                    FileReader reader(results[choice-1].string());
                }
                else{
                    std::cout<<"\nInvalid choice\n";
                }
            }
        }

        std::cout<<"\n\nType 'exit' to quit or press Enter to continue...\n\n\n";
        std::string input;
        std::getline(std::cin, input);
        for(auto &c : input) c=tolower(c);
            if(input=="exit") 
                break;
    }
    
    std::cout<<"\n\n\n******************************************************************************PRESS ENTER TO EXIT*****************************************************************************\n";
    std::cin.get();
}