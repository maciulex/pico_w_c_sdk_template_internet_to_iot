#ifndef MODULES_RUTINE_F
#define MODULES_RUTINE_F

namespace submodules{

    #if CONFIG_DHT20_ENABLED
        #include "libs/dht20/dht20.cpp"
        void dht_20_submodule() {

            DHT20::getMeasurement();
        }

    #endif

    #if CONFIG_BMP280_ENABLED
        #include "libs/BMP280/BMP280_I2C.cpp"

        void bmp280_submodule() {
            BMP280::takeMesurment();
            //you can trigger measurment using readMeasurmentsRaw()
            //it will be stored for 8 bit in lastTemp, lastPressure for 32 bit temp, press
        }

    #endif


    #if CONFIG_DISPLAY_ENABLED
        void display_submodule() {
                
        }

    #endif

    #if CONFIG_DS18W20_ENABLED
        #include "libs/one_wire/api/one_wire.h"
        void  ds18w20_submodule() {
            CONFIG::one_wire.convert_temperature(CONFIG::ds18w20_address, true, true);
            for (int i = 0; i < CONFIG::ds18w20_amountOfDevices; i++) {
                CONFIG::ds18w20_address = CONFIG::one_wire.get_address(i);
                printf("%i Device Address: %02x%02x%02x%02x%02x%02x%02x%02x\n",i, 
                        CONFIG::ds18w20_address.rom[0], CONFIG::ds18w20_address.rom[1], CONFIG::ds18w20_address.rom[2], CONFIG::ds18w20_address.rom[3], 
                        CONFIG::ds18w20_address.rom[4], CONFIG::ds18w20_address.rom[5], CONFIG::ds18w20_address.rom[6], CONFIG::ds18w20_address.rom[7]);

                printf("%i Temperature: %3.5foC\n",i, CONFIG::one_wire.temperature(CONFIG::ds18w20_address));
            }
        }

    #endif
}
#endif