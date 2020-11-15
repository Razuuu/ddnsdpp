#include "curl_helper.h"
#include "ddnsd.h"

static size_t write_callback(void *contents, size_t size, size_t nmemb,
        void *userp) {
    ((std::string*) userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

curl_helper::curl_helper() {
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, std::string(CONFIG_DIR + ".cookies.ddns").c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
}

curl_helper::~curl_helper() {
    curl_easy_cleanup(curl);
}

std::string curl_helper::get_content(std::string url) {
    std::string read_buffer;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_perform(curl);

    return read_buffer;
}

long curl_helper::get_status() {
    long res;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res);

    return res;
}

std::string curl_helper::post_get_content(std::string url, std::string postfields) {
    std::string read_buffer;

    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_perform(curl);

    return read_buffer;
}

std::string curl_helper::urlencode(std::string raw) {
    return std::string(curl_easy_escape(curl, raw.c_str(), raw.length()));
}
