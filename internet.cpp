#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "pico/multicore.h"
#include "hardware/rtc.h"

#include "core0/main0.cpp"
#include "core1/main1.cpp"


int main()
{
    stdio_init_all();
    rtc_init();

    sleep_ms(3000);


    if (watchdog_caused_reboot()) {
        printf("Rebooted by Watchdog!\n");
        // Whatever action you may take if a watchdog caused a reboot
    }
    
    watchdog_enable(8000, 1);
    watchdog_update();

    multicore_launch_core1(core1_main);
    core0_main();
    
    
    while (true) {

    }
}
