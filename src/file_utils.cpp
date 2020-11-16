#include "file_utils.h"

#include <dirent.h>

bool is_dir(std::string dir) {
    DIR* d = opendir(dir.c_str());

    if (d) {
        closedir(d);
        return true;
    }

    return false;
}
