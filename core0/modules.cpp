#include "../config.cpp"

#ifndef INT_TO_BIN
#define INT_TO_BIN
    #define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
    #define BYTE_TO_BINARY(byte)  \
    (byte & 0x80 ? '1' : '0'), \
    (byte & 0x40 ? '1' : '0'), \
    (byte & 0x20 ? '1' : '0'), \
    (byte & 0x10 ? '1' : '0'), \
    (byte & 0x08 ? '1' : '0'), \
    (byte & 0x04 ? '1' : '0'), \
    (byte & 0x02 ? '1' : '0'), \
    (byte & 0x01 ? '1' : '0') 
#endif

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
    void rutine_submodules() {
        #if CONFIG_DHT20_ENABLED
            dht_20_submodule();
        #endif
        #if CONFIG_BMP280_ENABLED
            bmp280_submodule();
        #endif
        #if CONFIG_DISPLAY_ENABLED
        
        #endif
        #if CONFIG_DS18W20_ENABLED
            ds18w20_submodule();

        #endif
    
    }

    void init_submodules() {
        #if CONFIG_DHT20_ENABLED
            DHT20::fullINIT(CONFIG::i2c);
            uint8_t anwser = DHT20::getStatusWord();
            printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(anwser));
        #endif
        #if CONFIG_BMP280_ENABLED
            BMP280::init(CONFIG::i2c,false);
            BMP280::setOversampling(BMP280::OVERSAMPLING_TEMPERATURE::OSRS_T_16, BMP280::OVERSAMPLING_PRESSURE::OSRS_P_16);
            BMP280::setConfigRegister(BMP280::TIME_STANDBY::t500);
        #endif
        #if CONFIG_DISPLAY_ENABLED
        
        #endif
        #if CONFIG_DS18W20_ENABLED
            //GP15 - Pin 20 on Pi Pico
            CONFIG::one_wire.init();
            CONFIG::ds18w20_amountOfDevices = CONFIG::one_wire.find_and_count_devices_on_bus();
        #endif
    
    }
}   