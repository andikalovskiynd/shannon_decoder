#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <stdexcept>

void readDictionary (const std::string& filename, std::map<char, std::string>& encodeTable, std::map<std::string, char>& decodeTable);
void writeDictionary (const std::string& filename, std::map<char, std::string>& encodeTable);

// writes raw bytes to file (no transformation)
void writeBytes (const std::string& data, const std::string& filename);

// packs a string of '0' or '1' into bytes and writes with a 4 byte header
void writeBits(const std::string& bits, const std::string& filename);