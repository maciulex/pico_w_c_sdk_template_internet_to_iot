#ifndef report_f
#define report_f

#include "../request.cpp"
#include "../../../user/core1/report.cpp"
#include <string>

namespace INTERNET {

    void report() {
        #if INTERNET_PRINT_DEBUG
            printf("\nGET REPORT\n");
        #endif
        std::string data = "&action=report"+user_report_generation(); 
        simple_send_data(CONFIG::SERVER_MAIN_PATH, data);
    }

}

#endif