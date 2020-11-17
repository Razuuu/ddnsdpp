#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>

std::string read_config(const std::string& config_path, const std::string& config_key);

bool read_config_bool(const std::string& config_path, const std::string& config_key);

long read_config_long(const std::string& config_path, const std::string& config_key);

#endif /* !_CONFIG_H */
