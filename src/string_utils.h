#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include <string>
#include <vector>

void replace_all(std::string& source, std::string from, std::string to);
std::vector<std::string> split(std::string str, char token);

#endif /* !_STRING_UTILS_H */
