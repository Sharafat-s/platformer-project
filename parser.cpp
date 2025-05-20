#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

static std::vector<std::string> decode_level(const std::string &line) {
    std::vector<std::string> level;
    std::istringstream iss(line);
    std::string segment;

    while (std::getline(iss, segment, '|')) {
        std::string row;
        size_t i = 0;
        while (i < segment.length()) {
            int count = 0;
            while (i < segment.length() && std::isdigit(segment[i])) {
                count = count * 10 + (segment[i] - '0');
                ++i;
            }
            if (count == 0) count = 1;
            if (i < segment.length()) {
                char symbol = segment[i++];
                row.append(count, symbol);
            }
        }
        level.push_back(row);
    }
    return level;
}

namespace Parser {
    std::vector<std::vector<std::string>> parseRLL(const std::string& filename) {
        std::ifstream file(filename);
        std::vector<std::vector<std::string>> levels;
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return levels;
        }

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == ';') continue;
            levels.push_back(decode_level(line));
        }

        return levels;
    }
}

