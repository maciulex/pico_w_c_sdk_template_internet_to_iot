#ifndef REPORT_USER_GENERATION_F
#define REPORT_USER_GENERATION_F

#include <string>
#include "../../config.cpp"
#include "../data.cpp"
#include "../../communication/data.cpp"

namespace INTERNET {
    std::string user_report_generation() {
        char a[10];
        snprintf(a, sizeof(a),
            "%.6f    ",
            WAR_ZONE::dst18w20_temp[0]);
        std::string result = "&somedataheader=";
        result.append(a);
        printf("returned report data: %s\n\n", result.c_str());
        return result;
    }
}


#endif