#ifndef PING_REQUEST_F
#define PING_REQUEST_F

#include "../request.cpp"
#include "../../../config.cpp"

namespace INTERNET {

    void ping() {
        #if INTERNET_PRINT_DEBUG
            printf("\nGET PING\n");
        #endif
        simple_send_data(CONFIG::SERVER_MAIN_PATH, "&action=ping");
    }

}

#endif