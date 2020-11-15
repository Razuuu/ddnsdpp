#include "config.h"

#include <fstream>

std::string read_config(std::string config_path, std::string config_key) {
    std::fstream config;
    config.open(config_path, std::ios::in);
    std::string config_value;

    std::string real_config_key = config_key + " = ";

    if (config.is_open()) {
        while (getline(config, config_value)) {
            if (config_value.substr(0, real_config_key.length()) == real_config_key) {
                config.close();

                return config_value.substr(real_config_key.length());
            }
        }

        config.close();
    }

    return "";
}

bool read_config_bool(std::string config_path, std::string config_key) {
    std::string config_value = read_config(config_path, config_key);

    return config_value == "1" || config_value == "true";
}

long read_config_long(std::string config_path, std::string config_key) {
    try {
        return std::stol(read_config(config_path, config_key));
    } catch (std::invalid_argument& e) {
        return 0;
    }
}
