#ifndef TIME_REQUEST_F
#define TIME_REQUEST_F

#include "../../../config.cpp"
#include "../request.cpp"
#include <stdio.h>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "pico/types.h"


namespace INTERNET {

    void get_time_callback(char* result) {
        uint16_t i = 0;
        uint16_t data_counter = 0;
        bool data = false;

        uint16_t time[7];
        char cache[4]; 
        uint8_t cache_i = 0;
        while (result[i] != '\0') {
            //finding data start and data end point
            if (result[i] == '<' && result[i+1] == '|' && result[i+2] == '|'  && result[i+3] == '>') {
                if (data) {
                    time[data_counter] = atoi(cache);
                    break;            
                } 
                data = true;
                i += 4;
                continue;
            } 
            if (!data) {
                i++;
                continue;
            }

            //processing data;
            if (result[i] == '-') {
                time[data_counter] = atoi(cache);
                cache[0] = 0;
                cache[1] = 0;
                cache[2] = 0;
                cache[3] = 0;
                cache_i = 0;
                i++;
                data_counter++;
                continue;
            }
            cache[cache_i] = result[i];
            cache_i += 1;
            i++;
        }

        datetime_t t = {
                .year  = (int16_t)time[3],
                .month = (int8_t)time[4],
                .day   = (int8_t)time[5],
                .dotw  = (int8_t)time[6], // 0 is Sunday, so 5 is Friday
                .hour  = (int8_t)time[0],
                .min   = (int8_t)time[1],
                .sec   = (int8_t)time[2]
        };
        
        #if INTERNET_PRINT_DEBUG
            printf("\n%d %d %d %d %d %d %d\n", time[0], time[1], time[2], time[3], time[4], time[5], time[6]);
        #endif
        
        if (rtc_set_datetime(&t)) {
            core1_watch_dog();
            CONFIG::TIME_INITIETED = true;
        }
        core1_watch_dog();
        #if INTERNET_PRINT_DEBUG
            printf("GET_TIME_END\n");
        #endif
    }

    void get_time() {
        CONFIG::TIME_INITIETED = false;
        
        #if INTERNET_PRINT_DEBUG
            printf("\nGET TIME\n");
        #endif

        simple_send_data(CONFIG::SERVER_MAIN_PATH, "&action=getTime", (void *)get_time_callback);
    }

}

#endif