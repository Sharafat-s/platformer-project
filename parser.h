#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

namespace Parser {
    std::vector<std::vector<std::string>> parseRLL(const std::string& filepath);

    std::vector<std::vector<std::string>> parse_rll_file(const std::string& filepath);

    std::vector<std::string> decode_rll_line(const std::string& line);
}

#endif // PARSER_H
