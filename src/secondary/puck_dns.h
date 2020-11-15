#ifndef _PUCK_DNS_H
#define _PUCK_DNS_H

#include "../curl_helper.h"
#include "secondary_dns.h"

#define PUCK_BASE_URL std::string("https://puck.nether.net/dns/")

class puck_dns : public secondary_dns {

    public:
        puck_dns(std::string username, std::string password);
        virtual ~puck_dns();
        bool login() override;
        bool update(std::string domain, std::string ip) override;

    private:
        curl_helper* curl;
        std::string username;
        std::string password;
};

#endif /* !_PUCK_DNS_H */
