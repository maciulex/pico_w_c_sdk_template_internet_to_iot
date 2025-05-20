#ifndef GET_PARAM_PROCESSING_USER_F
#define GET_PARAM_PROCESSING_USER_F
#include "pico/stdlib.h"
#include <string>

namespace INTERNET {


    std::string user_process_get_request(std::string head_name, std::string head_value) {
        printf("\nFOUND HEADER NAME: %s\n VALUE %s \n\n", head_name.c_str(), head_value.c_str());

        
        return "";
    }
}
#endif 