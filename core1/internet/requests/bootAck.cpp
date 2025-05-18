#ifndef BOOT_ACK_REQUEST_F
#define BOOT_ACK_REQUEST_F

#include "../request.cpp"
#include "../../../config.cpp"

namespace INTERNET {

    void boot_ack() {
        #if INTERNET_PRINT_DEBUG
            printf("bootack\n");
        #endif
        simple_send_data(CONFIG::SERVER_MAIN_PATH, "&action=bootAck&version="+CONFIG::PICO_VERSION);
    }
}
#endif