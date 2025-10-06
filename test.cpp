#include <gtest/gtest.h>

#include "decoder.h"
#include "encoder.h"
#include "dictionary.h"
#include "reader_and_writer.h"

#include <cstdio>

void runEncodeDecode (const std::string& input) {
    const std::string inputFile = "test_input.bin";
    const std::string encodedFile = "test_output.bin";
    const std::string dictFile = "test_dictionary.csv";
    const std::string decodedFile = "test_decoded.bin";

    writeBytes(input, inputFile);

    // coding
    Dictionary dict;
    std::string origin = readBinary(inputFile);
    auto symProb = calculateProb(origin);
    sortByProb(symProb);
    auto symProbLen = calculateLength(symProb); 

    assignCodes(symProbLen, dict.encodeTable, dict.decodeTable);

    writeDictionary(dictFile, dict.encodeTable);
    std::string bitString = encode(origin, dict.encodeTable);
    writeBits(bitString, encodedFile);

    // decoding 
    readDictionary(dictFile, dict.encodeTable, dict.decodeTable);

    std::vector<bool> buffer = readBits(encodedFile);
    std::string decoded = decode(buffer, dict.decodeTable);

    writeBytes(decoded, decodedFile);

    //
    EXPECT_EQ(origin, decoded);

    std::remove(inputFile.c_str());
    std::remove(encodedFile.c_str());
    std::remove(dictFile.c_str());
    std::remove(decodedFile.c_str());
}

TEST(ShannonTest, EmptyInput) {
    runEncodeDecode("");
}

TEST(ShannonTest, ManyDifferent) {
    runEncodeDecode("ABCDEQJSOP");
}

TEST(ShannonTest, SingleSymbol) {
    runEncodeDecode("KKKKKKK");
}

TEST(ShannonTest, Hello) {
    runEncodeDecode("Hello world!");
}

TEST(ShannonTest, AlphabetEng) {
    runEncodeDecode("The quick brown fox jumps over the lazy dog");
}

TEST(ShannonTest, AlphabetRussian) {
    runEncodeDecode("Съешь ещё этих мягких французских булок, да выпей же чаю");
}

TEST(ShannonTest, ManyRepeated) {
    runEncodeDecode("qweqweqweqweqweqweqwe");
}

TEST(ShannonTest, Prayers) {
    runEncodeDecode("Пожалуйста, простите, что сдаю работы так поздно...");
}