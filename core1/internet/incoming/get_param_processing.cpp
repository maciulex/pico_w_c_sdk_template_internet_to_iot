#include "pico/stdlib.h"
#include <string>

namespace INTERNET {


    std::string process_get_request(std::string head_name, std::string head_value) {
        printf("\nFOUND HEADER NAME: %s\n VALUE %s \n\n", head_name.c_str(), head_value.c_str());

        return "";
    }
}