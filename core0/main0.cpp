#include "hardware/watchdog.h"

#include "modules.cpp"
#include "../../../user/core0/modules/modules.cpp"

void core0_main() {
    submodules::init_submodules();
    submodules::user_init_submodules();
    uint32_t core1_last_counter = 0;
    uint8_t  core1_strike       = 0;
    while (true) {

        submodules::rutine_submodules();
        watchdog_update();
        submodules::user_rutine_submodules();
        sleep_ms(100);
        
        #if CORE0_PRINT_HEARTBEAT
            printf("X");
        #endif
        if (core1_last_counter == core1_counter) {
            core1_strike += 1;
            printf("core1 lock strike \n");
            if (core1_strike > 5) {
                #if CONFIG_ON_CORE_1_STUCK == 1
                    while (true) continue; // reboot board by watchdog
                #endif
                #if CONFIG_ON_CORE_1_STUCK == 2
                    multicore_reset_core1();
                #endif
            }
        } else{
            core1_strike = 0;
        }
        core1_last_counter = core1_counter;
        watchdog_update();
    }
    return;
}