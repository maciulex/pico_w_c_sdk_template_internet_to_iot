#ifndef ping_f
#define ping_f

#include "../request.cpp"

namespace INTERNET {

    void ping() {
        #if INTERNET_PRINT_DEBUG
            printf("\nGET PING\n");
        #endif
        simple_send_data("/newSystem/traffic_control.php", "&action=ping");
    }

}

#endif