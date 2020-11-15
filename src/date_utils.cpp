#include "date_utils.h"

#include <ctime>
#include <iomanip>
#include <sstream>

std::string time_format(std::string format) {
    std::stringstream buffer;
    time_t rawtime = time(0);
    struct std::tm * ptm = localtime(&rawtime);

    buffer << std::put_time(ptm, format.c_str());

    return buffer.str();
}
