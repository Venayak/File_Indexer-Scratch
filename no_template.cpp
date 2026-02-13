#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

class HashTable {
    struct Node {
        std::string key;
        std::vector<std::string> values;
    };

    std::vector<std::vector<Node>> table;

    int hash_function(const std::string& key) const {
        int sum = 0;
        for (int i = 0; i < key.length(); i++) {
            sum += key[i]; //ASCII value of alphabet
        }
        return sum % 100;
    }

public:
    HashTable() {
    table.resize(100); //100 drawers
    }

    void insert(const std::string& key, const std::string& value) {
        int index = hash_function(key);

        for (int i = 0; i < table[index].size(); i++) { //for each node in drawer number(index)
            if (table[index][i].key == key) {
                table[index][i].values.push_back(value);
                return;
            }
        }

        Node newNode;
        newNode.key = key;
        newNode.values.push_back(value);
        table[index].push_back(newNode);
    }

    void print() const {
        for (int i = 0; i < table.size(); i++) {
            for (int j = 0; j < table[i].size(); j++) {
                std::cout << table[i][j].key << '\n';
                for (int k = 0; k < table[i][j].values.size(); k++) {
                    std::cout << "  " << table[i][j].values[k] << '\n';
                }
                std::cout << '\n';
            }
        }
    }

    bool search(const std::string& key, std::vector<std::string>& result) const {
        int index = hash_function(key);
        for (int i = 0; i < table[index].size(); i++) {
            if (table[index][i].key == key) {
                result = table[index][i].values;
                return true;
            }
        }
        return false;
    }
};

class FileIndexer {
    fs::path root_path;
    HashTable index;

    // 🔹 ADDED
    std::vector<std::string> ignored_dirs;

    bool is_ignored(const fs::path& path) const {
        for (const auto& ignored : ignored_dirs) {
            if (path.string().find(ignored) != std::string::npos) { //no position
                return true;
            }
        }
        return false;
    }

public:
    void set_root(const fs::path& root) {
        root_path = root;
    }

    // 🔹 ADDED
    void add_ignored_directory(const std::string& name) {
        ignored_dirs.push_back(name);
    }

    void build_index() {
        for (const fs::directory_entry& entry :
             fs::recursive_directory_iterator(
                 root_path,
                 fs::directory_options::skip_permission_denied)) {

            if (!entry.is_regular_file())
                continue;

            // 🔹 ADDED
            if (is_ignored(entry.path()))
                continue;

            std::string filename =
                entry.path().filename().generic_string();

            std::string filepath =
                fs::absolute(entry.path()).generic_string();

            index.insert(filename, filepath);
        }
    }

    void print_index() const {
        index.print();
    }

    void search_file(const std::string& filename) const {
        std::vector<std::string> paths;
        if (index.search(filename, paths)) {
            std::cout << "Found:\n";
            for (const auto& p : paths) {
                std::cout << "  " << p << '\n';
            }
        } else {
            std::cout << "File not found.\n";
        }
    }
};

int main() {
    std::cout << "\033[2J\033[1;1H";

    FileIndexer indexer;

    std::cout << "Enter root directory to index:\n";
    std::string root;
    std::getline(std::cin, root);

    if (root.empty()) {
        root = ".";
    }

    std::cout << "\nHow many directory names do you want to ignore?\n";
    int n;
    std::cin >> n;
    std::cin.ignore();

    for (int i = 0; i < n; i++) {
        std::string ignore;
        std::cout << "Enter directory name to ignore #" << (i + 1) << ":\n";
        std::getline(std::cin, ignore);

        if (!ignore.empty()) {
            indexer.add_ignored_directory(ignore);
        }
    }

    indexer.set_root(root);
    indexer.build_index();

    std::cout << "\nEnter filename to search:\n";
    std::string filename;
    std::getline(std::cin, filename);

    indexer.search_file(filename);

    return 0;
}