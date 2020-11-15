#include "bind9_zone_parser.h"
#include "string_utils.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

bind9_zone_parser::bind9_zone_parser(std::string file) {
    this->file = file;
}

bind9_zone_parser::~bind9_zone_parser() {
}

long bind9_zone_parser::get_serial() {
    std::string tmp;
    std::fstream f;
    f.open(file, std::ios::in);

    long linenum = 0;
    bool soa_found = false;
    long cur_brackets = 0;
    std::string type = "";

    while (getline(f, tmp, '\n')) {
        linenum++;

        if (cur_brackets == 0) {
            soa_found = false;
        }

        std::replace(tmp.begin(), tmp.end(), '\t', ' '); // replace tabs with spaces

        std::vector<std::string> broken = split(tmp, ' ');

        for (std::string field : broken) {
            if (field == "SOA") {
                soa_found = true;
            } else if (field == "(") {
                cur_brackets++;
            } else if (field == ")") {
                if (cur_brackets > 0) {
                    cur_brackets--;
                } else {
                    std::cerr << "Found closing bracket without opening bracket!" << std::endl;
                }
            } else if (field.find(";") != std::string::npos) { // comment at the end of line
                goto nextline;
            } else if (soa_found) {
                try {
                    return stol(field); // serial is first field after "SOA"
                } catch (std::invalid_argument &e) {
                    continue;
                }
            }
        }

        nextline:
            continue;
    }

    return -1;
}
