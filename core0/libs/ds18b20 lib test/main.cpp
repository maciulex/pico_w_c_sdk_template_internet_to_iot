#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "modules/one_wire/api/one_wire.h"

int main() {
    stdio_init_all();
    One_wire one_wire(15); //GP15 - Pin 20 on Pi Pico
    one_wire.init();
    uint8_t amountOfDevices = one_wire.find_and_count_devices_on_bus();
    rom_address_t address{};
    for (int i = 0; i < amountOfDevices; i++) {
        one_wire.set_resolution(one_wire.get_address(i), 12);
    }

    while (true) {
        one_wire.convert_temperature(address, true, true);
        for (int i = 0; i < amountOfDevices; i++) {
            address = one_wire.get_address(i);
            printf("%i Device Address: %02x%02x%02x%02x%02x%02x%02x%02x\n",i, 
                    address.rom[0], address.rom[1], address.rom[2], address.rom[3], 
                    address.rom[4], address.rom[5], address.rom[6], address.rom[7]);

            printf("%i Temperature: %3.5foC\n",i, one_wire.temperature(address));
        }
        sleep_ms(100);
    }
    return 0;
}