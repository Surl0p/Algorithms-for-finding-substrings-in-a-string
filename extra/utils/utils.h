#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>

bool readFileToString(const char* filename, std::string& content);
bool readPatternsFromFile(const char* filename, std::vector<std::string>& patterns);

#endif // UTILS_H