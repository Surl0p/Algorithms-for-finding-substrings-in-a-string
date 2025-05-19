#include "utils.h"

bool readFileToString(const char* filename, std::string& content) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file '" << filename << "'!" << std::endl;
        return false;
    }
    content.assign((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
    if (file.bad()) {
        std::cerr << "Error while reading file '" << filename << "'!" << std::endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool readPatternsFromFile(const char* filename, std::vector<std::string>& patterns) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for reading patterns." << std::endl;
        return false;
    }

    patterns.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            patterns.push_back(line);
        }
    }

    file.close();
    return true;
}