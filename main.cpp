#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <string_view>

#include "dictionary.h"
#include "encoder.h"
#include "decoder.h"
#include "reader_and_writer.h"

enum class Mode {
    ENCODE,
    DECODE
};

Mode checkArguments(const std::vector<std::string_view>& arguments) {
    for (auto argument : arguments) {
        if (argument == "-e") return Mode::ENCODE;
        if (argument == "-d") return Mode::DECODE;
    }
    throw std::runtime_error("INVALID ARGUMENTS");
}

int main (int argc, char* argv[]) {
    try {
        std::vector<std::string_view> arguments(argv + 1, argv + argc);

        Mode mode = checkArguments(arguments);

        std::string inputFile = "input.bin";
        std::string outputFile = "output.bin";
        std::string dictFile = "dictionary.csv";

        for (size_t i = 1; i < arguments.size(); ++i) {
            if (arguments[i] == "--input" && i + 1 < arguments.size()) inputFile = std::string(arguments[++i]);
            if (arguments[i] == "--output" && i + 1 < arguments.size()) outputFile = std::string(arguments[++i]);
            if (arguments[i] == "--dict" && i + 1 < arguments.size()) dictFile = std::string(arguments[++i]);
        }

        Dictionary dict; 
        if (mode == Mode::ENCODE) {
            std::string origin = readBinary(inputFile);
            auto symProb = calculateProb(origin);
            sortByProb(symProb);
            auto symProbLen = calculateLength(symProb); 

            assignCodes(symProbLen, dict.encodeTable, dict.decodeTable);

            writeDictionary(dictFile, dict.encodeTable);
            std::string bitString = encode(origin, dict.encodeTable);
            writeBits(bitString, outputFile);
        }

        else if (mode == Mode::DECODE) {
            readDictionary(dictFile, dict.encodeTable, dict.decodeTable);

            std::vector<bool> buffer = readBits(inputFile);
            std::string decodedString = decode(buffer, dict.decodeTable);

            writeBytes(decodedString, outputFile);
        }

        return 0;
    }

    catch (const std::exception& ex) {
        std::cerr << "ERR: " << ex.what() << std::endl;
        return 1;
    }
}