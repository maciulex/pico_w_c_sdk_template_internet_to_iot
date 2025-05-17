


#include "hardware/watchdog.h"
#include "modules.cpp"

void core0_main() {
    submodules::init_submodules();

    while (true) {

        submodules::rutine_submodules();
    }
    return;
}