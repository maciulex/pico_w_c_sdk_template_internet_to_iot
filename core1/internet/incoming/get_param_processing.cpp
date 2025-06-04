#ifndef GET_PARAM_PROCESSING_F
#define GET_PARAM_PROCESSING_F

#include "pico/stdlib.h"
#include <string>
#include <cstring>
#include "../../../config.cpp"
#include "../../../user/core1/get_param_processing_user.cpp"
#if CONFIG_PZEM004_ENABLED
    #include "../../../user/core0/libs/build_in/pzem/pzem.cpp"
#endif
namespace INTERNET {


    std::string process_get_request(std::string head_name, std::string head_value) {
        printf("\nFOUND HEADER NAME: %s\n VALUE %s \n\n", head_name.c_str(), head_value.c_str());

        if (strncmp(head_name.c_str(), "resetTime" , 9)) {
            CONFIG::TIME_INITIETED = false;
        }
        #if CONFIG_PZEM004_ENABLED
            if (head_name == "pzem_reset_power") {
                if (pzem::reset()) {
                    printf("reset done good\n");
                } else {
                    printf("reset somthing gone wrong\n");
                }
            }
        #endif
        return user_process_get_request(head_name, head_value);
    }
}

#endif 