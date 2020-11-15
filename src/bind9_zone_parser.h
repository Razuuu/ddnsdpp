#ifndef _BIND9_ZONE_PARSER_H
#define _BIND9_ZONE_PARSER_H

#include <string>

class bind9_zone_parser {
    public:
        bind9_zone_parser(std::string file);
        ~bind9_zone_parser();
        long get_serial();
    private:
        std::string file;
};

#endif /* !_BIND9_ZONE_PARSER */
