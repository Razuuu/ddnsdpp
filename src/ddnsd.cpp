#include "config.h"
#include "curl_helper.h"
#include "date_utils.h"
#include "ddnsd.h"
#include "file_utils.h"
#include "inet_utils.h"
#include "string_utils.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc > 1) {
        if(std::string("--version").compare(argv[1]) == 0) {
            std::cout << DDNSD_VERSION << std::endl;
            std::cout << LICENSE << std::endl;
            return 0;
        }
    }

    std::cout << "Starting " << DDNSD_VERSION << std::endl;

    if (!is_dir(CONFIG_DIR)) {
        if (mkdir(CONFIG_DIR.c_str(), 640) == -1) {
            std::cerr << "Directory " << CONFIG_DIR << " doesn't exist"
                    << " and couldn't be created!" << std::endl;
            return 1;
        }
    }

    std::fstream f;

    curl_helper curl;

    if (!is_inet4_address(read_config(OLDIP, "oldip"))) {
        f.open(OLDIP, std::ios::out);
        f << "oldip = " << curl.get_content(V4_API) << std::endl;
        f.close();
    }

    if (!is_inet6_address(read_config(OLDIP6, "oldip6"))) {
        f.open(OLDIP6, std::ios::out);
        f << "oldip6 = " << curl.get_content(V6_API) << std::endl;
        f.close();
    }

    // Main settings
    long update_freq = read_config_long(CONFIG, "update_freq");
    std::string post_update_cmd = read_config(CONFIG, "post_update_cmd");

    f.open(PIDFILE, std::ios::out);
    f << getpid() << std::endl;
    f.close();

    std::cout << "Waiting for IP change..." << std::endl;

    while (true) {
        std::string oldip = read_config(OLDIP, "oldip");
        std::string oldip6 = read_config(OLDIP6, "oldip6");

        std::string ip = curl.get_content(V4_API);
        std::string ip6 = curl.get_content(V6_API);

        if (!is_inet4_address(ip)) {
            std::cerr << "Failed to get valid IPv4 address!" << std::endl;
            ip = oldip;
        }

        if (!is_inet6_address(ip6)) {
            std::cerr << "Failed to get valid IPv6 address!" << std::endl;
            ip6 = oldip6;
        }

        if (ip != oldip || ip6 != oldip6) {
            std::cout << "Detected IP change!" << std::endl;
            std::cout << "IPv4: " << oldip << " -> " << ip << std::endl;
            std::cout << "IPv6: " << oldip6 << " -> " << ip6 << std::endl;

            f.open(OLDIP, std::ios::out);
            f << "oldip = " << ip << std::endl;
            f.close();
            f.open(OLDIP6, std::ios::out);
            f << "oldip6 = " << ip6 << std::endl;
            f.close();

            std::cout << "Command executed: " << post_update_cmd.c_str() << std::endl;

            system(post_update_cmd.c_str());
        }

        sleep(update_freq);
    }
}
