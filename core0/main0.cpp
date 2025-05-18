#include "hardware/watchdog.h"

#include "modules.cpp"
#include "user_modules/modules.cpp"

void core0_main() {
    submodules::init_submodules();
    submodules::user_init_submodules();

    while (true) {

        submodules::rutine_submodules();
        submodules::user_rutine_submodules();
        sleep_ms(100);
        
        #if CORE0_PRINT_HEARTBEAT
            printf("X");
        #endif
    }
    return;
}