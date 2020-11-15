#ifndef _CURL_HELPER_H
#define _CURL_HELPER_H

#include <cstddef>
#include <curl/curl.h>
#include <string>

class curl_helper {
    public:
        curl_helper();
        ~curl_helper();
        std::string get_content(std::string url);
        long get_status();
        std::string post_get_content(std::string url, std::string postfields);
        std::string urlencode(std::string raw);

    private:
        CURL* curl;
};

#endif /* _CURL_HELPER_H */
