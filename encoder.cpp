#include "encoder.h"

std::string readBinary (const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("COULD NOT OPEN BIN FILE FOR READING");

    std::string result;
    file.seekg(0, std::ios::end);
    std::streampos size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size > 0) {
        result.resize(static_cast<size_t>(size));
        file.read(&result[0], size);
    }
    return result;
}

std::vector<std::pair<char, double>> calculateProb (const std::string& text) {
    std::map<char, size_t> counts;

    for (char c : text) {
        counts[c]++;
    }

    size_t totalSize = text.size();

    // form a vector < symbol , probability >
    std::vector<std::pair<char, double>> probabilities;
    for (const auto& [symbol, count] : counts) {
        probabilities.push_back({symbol, static_cast<double>(count) / totalSize});
    }

    return probabilities;
}

bool comparePairs (const std::pair<char, double>& a, const std::pair<char, double>& b) {
    return a.second > b.second;
}

void sortByProb (std::vector<std::pair<char, double>>& symbols) {
    std::sort(symbols.begin(), symbols.end(), comparePairs);
}

std::vector<std::tuple<char, double, int>> calculateLength (const std::vector<std::pair<char, double>>& symProb) {
    std::vector<std::tuple<char, double, int>> result;

    // ONLY IF THERE IS SINGE SYMBOL
    if (symProb.size() == 1) {
        result.push_back({symProb[0].first, symProb[0].second, 1});
        return result;
    }

    for (const auto& [symbol, prob] : symProb) {
        int length = static_cast<int>(std::ceil(-std::log2(prob)));
        result.push_back({symbol, prob, length});
    }

    return result;
}

void assignCodes(std::vector<std::tuple<char, double, int>> symProbLen, std::map<char, std::string>& encodeTable, std::map<std::string, char>& decodeTable) {
    double Fi = 0.0;

    for (const auto& [sym, prob, len] : symProbLen) {
        double f = Fi;
        std::string code;

        // generating the code from Fi with "len" length
        for (int i = 0; i < len; ++i) {
            f *= 2;
            int bit = static_cast<int>(f);
            code += (bit ? '1' : '0');
            f -= bit;
        }

        encodeTable[sym] = code;
        decodeTable[code] = sym;

        Fi += prob;
    }
}

std::string encode(const std::string& input, const std::map<char, std::string>& encodeTable) {
    std::string bitString;

    for (char c : input) {
        auto encoded = encodeTable.find(c);
        if (encoded == encodeTable.end()) throw std::runtime_error("CHARACTER IS NOT ENCODED");

        const std::string& code = encoded->second;
        for (char bitChar : code) {
            if (bitChar != '0' && bitChar != '1') throw std::runtime_error("INVALID BIT CHARACTER IN ENCODE TABLE");
            bitString += bitChar;
        }
    }

    return bitString;
}