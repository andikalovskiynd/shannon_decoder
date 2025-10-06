#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <tuple>
#include <cmath>

// Step 0: read binary file to string
std::string readBinary (const std::string& filename);

// Step 1: estimation of each symbol probability
std::vector<std::pair<char, double>> calculateProb (const std::string& text);

// Step 2: sort by descending of probabilities
bool comparePairs (const std::pair<char, double>& a, const std::pair<char, double>& b);
void sortByProb (std::vector<std::pair<char, double>>& symbols);

// Step 3: calculate each symbol code length 
std::vector<std::tuple<char, double, int>> calculateLength (const std::vector<std::pair<char, double>>& symProb);

// Step 4: assigning codes
void assignCodes(std::vector<std::tuple<char, double, int>> symProbLen, std::map<char, std::string>& encodeTable, std::map<std::string, char>& decodeTable);

// Step 5: encoding
std::string encode(const std::string& input, const std::map<char, std::string>& encodeTable);