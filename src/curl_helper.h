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

    private:
        CURL* curl;
};

#endif /* _CURL_HELPER_H */
