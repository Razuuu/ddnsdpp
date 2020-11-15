#ifndef _DDNS_H
#define _DDNS_H

#include <string>

#define MULTILINE(...) #__VA_ARGS__

#define DDNSD_VERSION "DDNSD++ 1.0"

#define V4_API "http://v4.ident.me"
#define V6_API "http://v6.ident.me"

#define CONFIG_DIR std::string("/etc/ddns/")
#define CONFIG CONFIG_DIR + std::string("ddnsd.conf")
#define CONFIG_VERSION 1
#define OLDIP CONFIG_DIR + std::string(".oldip.ddns")
#define OLDIP6 CONFIG_DIR + std::string(".oldip6.ddns")

#define PIDFILE "/run/ddnsd.pid"

#define DATE_FORMAT "yyyyMMdd"

#endif /* !_CONFIG_H */
