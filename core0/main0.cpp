#include "hardware/watchdog.h"

#include "../../../user/core0/modules/build_in/modules.cpp"
#include "../../../user/core0/modules/modules.cpp"

time_t last_lock_check = 0;
void check_if_core_1_locked(time_t * this_cycle_time) {
    if (last_lock_check != 0 || *this_cycle_time-last_lock_check<9) return;
    last_lock_check = *this_cycle_time;
    
    while (CONFIG::lock_core1_last_activity) continue;
    CONFIG::lock_core1_last_activity = true;
    
    if (*this_cycle_time > 0 && CONFIG::core1_last_activity > 0 && 
        *this_cycle_time >= CONFIG::core1_last_activity && *this_cycle_time-3000000 < CONFIG::core1_last_activity ) {
        if (*this_cycle_time-CONFIG::core1_last_activity > 60) { // 60 SEC
            printf("60 SEC UP RESSETING \n");
            #if CONFIG_ON_CORE_1_STUCK == 1
                while (true) continue; // reboot board by watchdog
            #endif
            #if CONFIG_ON_CORE_1_STUCK == 2
                multicore_reset_core1();
            #endif
        }
    } else {
        printf("this cycle: %lld\n\n", (long long) *this_cycle_time);
        printf("last activity: %lld\n\n", (long long) CONFIG::core1_last_activity);
        printf("\n\ntime error\n\n");
    }
    CONFIG::lock_core1_last_activity = false;
}

void core0_main() {
    submodules::init_submodules();
    submodules::user_init_submodules();
    uint32_t core1_last_counter = 0;
    uint8_t  core1_strike       = 0;
    bool last_time_init = CONFIG::TIME_INITIETED;
    while (true) {
        
        datetime_t time_now;
        rtc_get_datetime(&time_now);
        time_t this_cycle_time = get_mktime(&time_now);
        
        #if CORE0_PRINT_HEARTBEAT
            printf("X");
        #endif
        
        watchdog_update();
        check_if_core_1_locked(&this_cycle_time);
        watchdog_update();
        submodules::rutine_submodules(this_cycle_time);
        watchdog_update();
        submodules::user_rutine_submodules(this_cycle_time);
        watchdog_update();
    }
    return;
}