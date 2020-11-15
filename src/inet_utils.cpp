#include "inet_utils.h"

#include <arpa/inet.h>

bool is_inet4_address(std::string str) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, str.c_str(), &(sa.sin_addr)) != 0;
}

bool is_inet6_address(std::string str) {
    struct sockaddr_in6 sa;
    return inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr)) != 0;
}
