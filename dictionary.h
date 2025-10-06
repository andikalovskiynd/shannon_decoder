#pragma once

#include <iostream>
#include <map>
#include <string>

struct Dictionary {
    std::map<char, std::string> encodeTable;
    std::map<std::string, char> decodeTable;
};