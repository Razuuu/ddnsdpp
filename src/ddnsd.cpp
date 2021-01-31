#include "bind9_zone_parser.h"
#include "config.h"
#include "curl_helper.h"
#include "date_utils.h"
#include "ddnsd.h"
#include "file_utils.h"
#include "inet_utils.h"
#include "secondary/he_dns.h"
#include "secondary/puck_dns.h"
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
    std::vector<std::string> zones = split(read_config(CONFIG, "zones"), ',');
    std::string post_update_cmd = read_config(CONFIG, "post_update_cmd");

    // PuckDNS settings
    bool use_puckdns = read_config_bool(CONFIG, "use_puckdns");
    long puckdns_mode = read_config_long(CONFIG, "puckdns_mode");
    std::vector<std::string> puckdns_domains = split(read_config(CONFIG, "puckdns_domains"), ',');
    puck_dns puckdns(read_config(CONFIG, "puckdns_username"), read_config(CONFIG, "puckdns_password"));

    // HE.net DNS settings
    bool use_hedns = read_config_bool(CONFIG, "use_hedns");
    long hedns_mode = read_config_long(CONFIG, "hedns_mode");
    std::vector<std::string> hedns_domains = split(read_config(CONFIG, "hedns_domains"), ',');
    he_dns hedns(read_config(CONFIG, "hedns_username"), read_config(CONFIG, "hedns_password"));

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

            if (use_puckdns) {
                for (std::string puckdomain : puckdns_domains) {
                    puckdns.update(puckdomain,
                            puckdns.get_ip_for_mode(puckdns_mode, ip, ip6));
                }
            }

            if (use_hedns) {
                for (std::string hedomain : hedns_domains) {
                    hedns.update(hedomain,
                            hedns.get_ip_for_mode(hedns_mode, ip, ip6));
                }
            }

            for (std::string zone : zones) {
                std::vector<std::string> zone_name_path;
                zone_name_path = split(zone, ':');
                std::string zone_path;
                std::string zone_name;

                if (zone_name_path.size() == 2) {
                    zone_name = zone_name_path.at(0);
                    zone_path = zone_name_path.at(1);
                } else {
                    std::cerr << "Zone entry " << zone << " is not in format name:path" << std::endl;
                    continue;
                }

                f.open(zone_path, std::ios::in);

                if (f.is_open()) {
                    f.close();

                    bind9_zone_parser parser(zone_path);
                    long serial_parsed = parser.get_serial();
                    std::string current_serial = std::to_string(serial_parsed);

                    if (current_serial.length() == 10) {
                        std::string current_serial_date = current_serial.substr(0, 8);
                        int version = atoi(current_serial.substr(9).c_str());
                        std::string current_date = time_format("%Y%m%d");

                        if (current_serial_date != current_date) {
                            version = 0;
                        }

                        version++;

                        if (version > 99) {
                            version = 1;
                        }

                        std::string version_str = std::to_string(version);
                        std::string new_serial = current_date + (version >= 10 ? version_str : "0" + version_str);

                        std::string dnszone;
                        f.open(zone_path, std::ios::in);
                        getline(f, dnszone, '\0');
                        f.close();

                        replace_all(dnszone, current_serial, new_serial);

                        f.open(zone_path, std::ios::out);
                        f << dnszone;
                        f.close();
                    } else {
                        std::cerr << "Serial " << current_serial << " of zone " << zone << " is not in format YYMMDDVV" << std::endl;
                        std::cerr << "Trying without updating serial!" << std::endl;
                    }

                    std::string dnszone;
                    f.open(zone_path, std::ios::in);
                    getline(f, dnszone, '\0');
                    f.close();

                    if (ip != oldip) {
                        replace_all(dnszone, oldip, ip);
                    }

                    if (ip6 != oldip6) {
                        replace_all(dnszone, oldip6, ip6);
                    }

                    f.open(zone_path, std::ios::out);
                    f << dnszone;
                    f.close();

                    std::cout << "Zone " << zone_name << " was updated!" << std::endl;
                } else {
                    std::cerr << "Failed to open zonefile " << zone_path << std::endl;
                    continue;
                }
            }

            f.open(OLDIP, std::ios::out);
            f << "oldip = " << ip << std::endl;
            f.close();
            f.open(OLDIP6, std::ios::out);
            f << "oldip6 = " << ip6 << std::endl;
            f.close();

            system(post_update_cmd.c_str());
        }

        sleep(update_freq);
    }
}
