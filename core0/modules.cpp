#include "../config.cpp"
#include "modules_rutine.cpp"

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