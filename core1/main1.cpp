


#include "hardware/watchdog.h"
#include "internet/connection.cpp"
#include "internet/request.cpp"

#include "internet/requests/bootAck.cpp"
#include "internet/requests/getTime.cpp"
#include "internet/requests/ping.cpp"
#include "internet/requests/report.cpp"


#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

void * connections_to_execute[10];

bool add_connection_to_execute(void * pt, uint8_t offset = 0) {
    for (int i = offset; i < 10; i++) {
        if (connections_to_execute[i] == nullptr) {
            connections_to_execute[i] = pt;
            return true;
        }
    }
    return false;
}

void core1_main() {
    bool boot_ack = false;

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return;
    }

    for (int i = 0; i < 10; i++) {
        connections_to_execute[i] = nullptr;
    }
    add_connection_to_execute((void *)INTERNET::boot_ack);
    add_connection_to_execute((void *)INTERNET::get_time);

    //connections_to_execute[0] = (void *)INTERNET::boot_ack;
    //connections_to_execute[1] = (void *)INTERNET::get_time;

    datetime_t time_now;
    rtc_get_datetime(&time_now);

    
    
    uint8_t last_minute_ping   = time_now.min;
    uint8_t last_minute_report = time_now.min;

    while (true) {
        //printf("Hello, worlds!\n");
        if (!INTERNET::INTERNET_CONNECTION) {
            INTERNET::connect_to_network();
        } else {
            sleep_ms(50);
            
            if (INTERNET::TIME_INITIETED) {
                rtc_get_datetime(&time_now);
                if (time_now.min % CONFIG::PING_TIME   == 0 && time_now.min != last_minute_ping) {
                    add_connection_to_execute((void *)INTERNET::ping, 2);
                    last_minute_ping = time_now.min;
                }
                #if INTERNET_ENABLE_AUTO_RAPORT
                    if (time_now.min % CONFIG::REPORT_TIME == 0 && time_now.min != last_minute_report) {
                        add_connection_to_execute((void *)INTERNET::report, 2);
                        last_minute_report = time_now.min;
                    }
                #endif
            }

            if (!INTERNET::ongoing_connection) {
                for (int i = 0; i < 10; i++) {
                    if (connections_to_execute[i] != nullptr) {
                        void (*f)() = (void (*)())connections_to_execute[i];
                        f();
                        connections_to_execute[i] = nullptr;
                        break;
                    }
                }
            }
            #if INTERNET_PRINT_DEBUG
                printf("z");
            #endif
            watchdog_update();
        }
        
        cyw43_arch_poll(); // ważne w NO_SYS=1

    }
}