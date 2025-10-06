#pragma once

#include <string>
#include <fstream>
#include <map>
#include <algorithm>

// Step 1: Read file into the vector
std::vector<bool> readBits (const std::string& filename);

// Step 2: find max code length for optimization
size_t getMaxCodeLength (const std::map<std::string, char>& decodeTable);

// Step 3: decode
std::string decode (const std::vector<bool>& buffer, const std::map<std::string, char>& decodeTable);