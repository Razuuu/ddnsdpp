#include "string_utils.h"

#include <sstream>

void replace_all(std::string& source, std::string from, std::string to) {
    if (!from.empty()) {
        std::string new_string;
        new_string.reserve(source.length());  // avoids a few memory allocations

        std::string::size_type lastPos = 0;
        std::string::size_type findPos;

        while (std::string::npos != (findPos = source.find(from, lastPos))) {
            new_string.append(source, lastPos, findPos - lastPos);
            new_string += to;
            lastPos = findPos + from.length();
        }

        // Care for the rest after last occurrence
        new_string += source.substr(lastPos);

        source.swap(new_string);
    }
}

std::vector<std::string> split(std::string str, char token) {
    std::istringstream ss(str);
    std::vector<std::string> broken;
    std::string tmp;

    while (std::getline(ss, tmp, token)) {
        broken.push_back(tmp);
    }

    return broken;
}
