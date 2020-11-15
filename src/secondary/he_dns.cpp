#include "he_dns.h"
#include "../string_utils.h"

#include <iostream>
#include <regex>

he_dns::he_dns(std::string username, std::string password) {
    this->curl = new curl_helper();
    this->username = username;
    this->password = password;
}

he_dns::~he_dns() {
    delete curl;
}

bool he_dns::element_with_attr_value_exists(
        std::string html, std::string attr, std::string value) {
    std::regex regex(attr + "=\"" + value + "\"");
    std::cmatch m;

    return std::regex_search(html.c_str(), html.c_str() + html.length(), m, regex);
}

std::string he_dns::value_of_element_with_attr_value(
        std::string html, std::string attr, std::string value) {
    std::regex regex(attr + "=\"" + value + "\" value=\"(.*)\"");
    std::cmatch m;

    if (std::regex_search(html.c_str(), html.c_str() + html.length(), m, regex)) {
        return split(m[1], '"').at(0); // cut off " matched by (.*)
    }

    return "";
}

bool he_dns::is_logged_in(std::string html) {
    return this->element_with_attr_value_exists(html, "name", "account");
}

bool he_dns::is_deleted(std::string html, std::string domain) {
    return !this->element_with_attr_value_exists(html, "name", domain);
}

bool he_dns::is_added(std::string html, std::string domain) {
    return !is_deleted(html, domain);
}

bool he_dns::login() {
    curl->get_content(HE_BASE_URL); // needed to get CGISESSID cookie
    return this->is_logged_in(curl->post_get_content(HE_BASE_URL,
            "email=" + curl->urlencode(username) + "&pass="
             + curl->urlencode(password) + "&submit=Login!"));
}

bool he_dns::update(std::string domain, std::string ip) {
    if (login()) {
        std::string html = curl->get_content(HE_BASE_URL);
        std::string account = value_of_element_with_attr_value(
                html, "name", "account");
        std::string delete_id = value_of_element_with_attr_value(
                html, "name", domain);

	if(is_deleted(curl->post_get_content(HE_BASE_URL, "account=" + curl->urlencode(account) +
                "&delete_id=" + curl->urlencode(delete_id) + "&remove_domain=1"), domain)) {
            if (is_added(curl->post_get_content(HE_BASE_URL, "add_slave=" + curl->urlencode(domain) +
                     "&master1=" + curl->urlencode(ip)), domain)) {
                std::cout << "HEDNS successfully updated " << domain << std::endl;
                return true;
            } else {
                std::cerr << "HEDNS failed updating " << domain << " (add failed)" << std::endl;
                return false;
            }
        } else {
            std::cerr << "HEDNS failed updating " << domain << " (delete failed)" << std::endl;
            return false;
        }
    }

    std::cerr << "HEDNS login failed" << std::endl;

    return false;
}
