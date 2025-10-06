#include "reader_and_writer.h"
#include <cstdint>

void readDictionary (const std::string& filename, std::map<char, std::string>& encodeTable, std::map<std::string, char>& decodeTable) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("COULD NOT OPEN DIC FILE");

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string symbolString;
        std::string codeString;

        if (!std::getline(ss, symbolString, ',') || !std::getline(ss, codeString, ',')) {
            throw std::runtime_error("INVALID FORMAT AT LINE " + std::to_string(lineNumber));
        }

        if (symbolString.size() != 1) throw std::runtime_error("INVALID SYMBOL AT LINE " + std::to_string(lineNumber));

        if (codeString.empty()) throw std::runtime_error("MISSING CODE AT LINE " + std::to_string(lineNumber));

        char symbol = symbolString[0];
        encodeTable[symbol] = codeString;
        decodeTable[codeString] = symbol;
    }

    if (encodeTable.empty()) throw std::runtime_error("EMPTY DIC FILE");
}

void writeDictionary (const std::string& filename, std::map<char, std::string>& encodeTable) {
    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("COULD NOT OPEN FILE FOR WRITING");

    for (const auto& [symbol, code] : encodeTable) {
        if (symbol == ',') {
            file << "\" , \"" << "," << code << "\n"; // processing "," as encoding symbol
        }

        else {
            file << symbol << "," << code << "\n";
        }
    }

    file.close();
}

void writeBytes (const std::string& data, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("COULD NOT OPEN FILE FOR WRITING");
    file.write(data.data(), data.size());
}

void writeBits (const std::string& bits, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("COULD NOT OPEN FILE FOR WRITING");

    uint32_t bitLen = static_cast<uint32_t>(bits.size());
    unsigned char header[4] = {static_cast<unsigned char>((bitLen >> 24) & 0xFF),
    static_cast<unsigned char>((bitLen >> 16) & 0xFF),
    static_cast<unsigned char>((bitLen >> 8) & 0xFF),
    static_cast<unsigned char>(bitLen & 0xFF)
    };

    file.write(reinterpret_cast<const char*>(header), 4);

    uint8_t byte = 0;
    int bitPos = 7;
    for (char b : bits) {
        if (b != '0' && b != '1') throw std::runtime_error("BITSTREAM CONTAINS NON-BINARY CHARACTER");

        if (b == '1') byte |= static_cast<uint8_t>(1u << bitPos);
        
        if (--bitPos < 0) {
            file.put(static_cast<char>(byte));
            byte = 0;
            bitPos = 7;
        }
    }

    if (bitPos != 7) file.put(static_cast<char>(byte));
}