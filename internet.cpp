#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "pico/multicore.h"
#include "hardware/rtc.h"

#include "core0/main0.cpp"
#include "core1/main1.cpp"


// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments






int main()
{
    stdio_init_all();
    rtc_init();

    sleep_ms(3000);

    // Initialise the Wi-Fi chip

    // I2C Initialisation. Using it at 400Khz.

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // Watchdog example code
    if (watchdog_caused_reboot()) {
        printf("Rebooted by Watchdog!\n");
        // Whatever action you may take if a watchdog caused a reboot
    }
    
    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(8000, 1);
    
    // You need to call this function at least more often than the 100ms in the enable call to prevent a reboot
    watchdog_update();

    multicore_launch_core1(core1_main);
    core0_main();
    
    
    while (true) {

    }
}
