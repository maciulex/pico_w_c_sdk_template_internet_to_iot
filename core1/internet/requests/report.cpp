#ifndef report_f
#define report_f

#include "../request.cpp"

namespace INTERNET {

    void report() {
        #if INTERNET_PRINT_DEBUG
            printf("\nGET REPORT\n");
        #endif
        simple_send_data("/newSystem/traffic_control.php", "&action=report");
    }

}

#endif