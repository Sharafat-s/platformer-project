#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

namespace Parser {
    std::vector<std::vector<std::string>> parseRLL(const std::string& filepath);

    // Parses RLL-encoded level file into a list of 2D char matrices (levels)
    std::vector<std::vector<std::string>> parse_rll_file(const std::string& filepath);

    // Decodes a single RLL-encoded line into a 2D level layout
    std::vector<std::string> decode_rll_line(const std::string& line);
}

#endif // PARSER_H
