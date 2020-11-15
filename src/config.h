#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>

std::string read_config(std::string config_path, std::string config_key);

bool read_config_bool(std::string config_path, std::string config_key);

long read_config_long(std::string config_path, std::string config_key);

#endif /* !_CONFIG_H */
