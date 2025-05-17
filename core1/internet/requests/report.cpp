#ifndef report_f
#define report_f

#include "../request.cpp"

namespace INTERNET {

    void report() {
        #if INTERNET_PRINT_DEBUG
            printf("\nGET REPORT\n");
        #endif
        simple_send_data(CONFIG::SERVER_MAIN_PATH, "&action=report");
    }

}

#endif