#ifndef _DDNS_H
#define _DDNS_H

#include <string>

#define MULTILINE(...) #__VA_ARGS__

#define V4_API "http://v4.ident.me"
#define V6_API "http://v6.ident.me"

#define LICENSE \
"Copyright (C) 2020  Tim Zimmermann <tim@linux4.de>\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU Affero General Public License as\n\
published by the Free Software Foundation, either version 3 of the\n\
License, or (at your option) any later version."

#define CONFIG_DIR std::string("/etc/ddns/")
#define CONFIG CONFIG_DIR + std::string("ddnsd.conf")
#define CONFIG_VERSION 1
#define OLDIP CONFIG_DIR + std::string(".oldip.ddns")
#define OLDIP6 CONFIG_DIR + std::string(".oldip6.ddns")

#define PIDFILE "/run/ddnsd.pid"

#define DATE_FORMAT "yyyyMMdd"

#endif /* !_CONFIG_H */
