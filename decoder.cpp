#include "decoder.h"
#include <cstdint>

std::vector<bool> readBits (const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("COULD NOT OPEN BIN FILE FOR READING");

    // a lot of problems occured here, so this header system was implemented 
    unsigned char header[4];
    if (!file.read(reinterpret_cast<char*>(header), 4)) throw std::runtime_error("ENCODED FILE TOO SHORT");

    uint32_t bitLen = (static_cast<uint32_t>(header[0]) << 24) |
    (static_cast<uint32_t>(header[1]) << 16) |
    (static_cast<uint32_t>(header[2]) << 8)  |
    (static_cast<uint32_t>(header[3]));

    std::vector<bool> buffer;
    buffer.reserve(bitLen);

    char byte;
    while (file.get(byte)) {
        uint8_t ubyte = static_cast<uint8_t>(byte);
        for (int i = 7; i >= 0; --i) {
            bool bit = (ubyte >> i) & 1;
            buffer.push_back(bit);
        }
    }

    if (buffer.size() < bitLen) throw std::runtime_error("ENCODED FILE TRUNCATED");
    if (buffer.size() > bitLen) buffer.resize(bitLen);

    return buffer;
}

size_t getMaxCodeLength (const std::map<std::string, char>& decodeTable) {
    size_t maxLen = 0;
    for (const auto& pair : decodeTable) {
        maxLen = std::max(maxLen, pair.first.size());
    }

    return maxLen;
}

std::string decode (const std::vector<bool>& buffer, const std::map<std::string, char>& decodeTable) {
    size_t maxLen = getMaxCodeLength(decodeTable);

    std::string result;
    std::string currentCode;
    size_t index = 0;

    while (index < buffer.size()) {
        size_t possibleLen = std::min(maxLen, buffer.size() - index); 

        for (size_t i = 0; i < possibleLen; ++i) {
            currentCode += buffer[index++] ? '1' : '0';

            auto found = decodeTable.find(currentCode);
            
            if (found != decodeTable.end()) {
                result += found->second;
                currentCode.clear();
                break;
            }
        }
    }

    if (!currentCode.empty()) throw std::runtime_error("INVALID OR INCOMPLETE CODE IN BUFFER");

    return result;
}
