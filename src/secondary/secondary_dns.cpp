#include "../inet_utils.h"
#include "secondary_dns.h"

// TODO: check in ddnsd.cpp if ipv6 connectivity actually works
std::string secondary_dns::get_ip_for_mode(
        long mode, std::string ip, std::string ip6) {
	if (((mode == 0 || mode == 2) && is_inet6_address(ip6)) ||
                (mode == 1 && !is_inet4_address(ip))) {
            return ip6;
        }

        return ip;
}
