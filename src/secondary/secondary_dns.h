#ifndef _SECONDARY_DNS_H
#define _SECONDARY_DNS_H

#include <string>

class secondary_dns {

    public:
        std::string get_ip_for_mode(long mode,
                std::string ip, std::string ip6);
        virtual bool login() = 0;
        virtual bool update(std::string domain, std::string ip) = 0;

};

#endif /* !_SECONDARY_DNS_H */
