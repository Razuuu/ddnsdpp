#include "puck_dns.h"

#include <iostream>

puck_dns::puck_dns(const std::string username, const std::string password) :
curl(curl_helper()), username(username), password(password) {}

bool puck_dns::login() {
    curl.post_get_content(PUCK_BASE_URL + "login", "username=" + username + "&password=" + password);
    return curl.get_status() == 302;
}

bool puck_dns::update(const std::string domain, const std::string ip) {
    if (this->login()) {
        curl.post_get_content(PUCK_BASE_URL + "dnsinfo/edit/" + domain, "domainname=" + domain + "&masterip=" + ip
                + "&aa=Y&submit=Submit");

        if (curl.get_status() == 302) {
            std::cout << "PuckDNS successfully updated " << domain << std::endl;
            return true;
        } else {
            std::cerr << "PuckDNS failed updating " << domain << std::endl;
            return false;
        }
    }

    std::cerr << "PuckDNS login failed" << std::endl;

    return false;
}
