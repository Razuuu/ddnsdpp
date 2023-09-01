#include "curl_helper.h"
#include "ddnsd.h"

static size_t write_callback(void *contents, size_t size, size_t nmemb,
        void *userp) {
    ((std::string*) userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

curl_helper::curl_helper() : curl(curl_easy_init()) {
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); // enable cookies without writing them to a file
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
}

curl_helper::~curl_helper() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

std::string curl_helper::get_content(std::string url) {
    std::string read_buffer;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_perform(curl);

    return read_buffer;
}
