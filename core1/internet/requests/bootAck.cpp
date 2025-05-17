#ifndef boot_ack_f
#define boot_ack_f

#include "../request.cpp"

namespace INTERNET {

    void boot_ack() {
        #if INTERNET_PRINT_DEBUG
            printf("bootack\n");
        #endif
        simple_send_data("/newSystem/traffic_control.php", "&action=bootAck");
    }
}
#endif