#ifndef _HE_DNS_H
#define _HE_DNS_H

#include "../curl_helper.h"
#include "secondary_dns.h"

#define HE_BASE_URL std::string("https://dns.he.net/")

class he_dns : public secondary_dns {

    public:
        he_dns(std::string username, std::string password);
        virtual ~he_dns();
        bool login() override;
        bool update(std::string domain, std::string ip) override;

    private:
        curl_helper* curl;
        std::string username;
        std::string password;

        bool element_with_attr_value_exists(
                std::string html, std::string attr, std::string value);
        std::string value_of_element_with_attr_value(
                std::string html, std::string attr, std::string value);
        bool is_logged_in(std::string html);
        bool is_deleted(std::string html, std::string domain);
        bool is_added(std::string html, std::string domain);
};

#endif /* !_HE_DNS_H */
