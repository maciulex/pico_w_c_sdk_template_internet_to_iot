#ifndef MODULES_RUTINE_F
#define MODULES_RUTINE_F

#include "../../../config.cpp"
#include "../../../communication/data.cpp"

namespace submodules{

    #if CONFIG_DHT20_ENABLED
        #include "libs/build_in/dht20/dht20.cpp"
        void dht_20_submodule() {

            DHT20::getMeasurement();
        }

    #endif

    #if CONFIG_BMP280_ENABLED
        #include "libs/build_in/BMP280/BMP280_I2C.cpp"

        void bmp280_submodule() {
            BMP280::takeMesurment();
            WAR_ZONE::bmp280_temp  = BMP280::temp;
            WAR_ZONE::bmp280_press = BMP280::press;
            #if CONFIG_BMP280_PRINT_AFTER_READ
                printf("TEMP BMP280: %f\n", ((float) BMP280::temp/ 100));
                printf("PRES BMP280: %i\n", BMP280::press);
            #endif
            //you can trigger measurment using readMeasurmentsRaw()
            //it will be stored for 8 bit in lastTemp, lastPressure for 32 bit temp, press
        }

    #endif


    #if CONFIG_DISPLAY_ENABLED
        #include "libs/build_in/display/display.cpp"
        void display_submodule() {
            // printf("runtime display 1\n");

            LCD_API::clear();
            // printf("runtime display 2\n");

            LCD_API::move_to(0,0);
            // printf("runtime display 3\n");

            LCD_API::put_str("TEST");
            // printf("runtime display 4\n");

            LCD_API::move_to(0,1);
            // printf("runtime display 5\n");
            LCD_API::put_str("TEST2");

            //LCD_API::put_str("TEST 2");
            // printf("runtime display 6\n");
            sleep_ms(1000);
            LCD_API::backlight_off();
            sleep_ms(1000);
            LCD_API::backlight_on();

        }

    #endif

    #if CONFIG_DS18W20_ENABLED
        #include "libs/build_in/one_wire/api/one_wire.h"
        void  ds18w20_submodule() {
            if (CONFIG::ds18w20_amountOfDevices < CONFIG_ONE_WIRE_EXPECTED_AMOUNT_OF_DEVICES) {
                CONFIG::one_wire.init();
                CONFIG::ds18w20_amountOfDevices = CONFIG::one_wire.find_and_count_devices_on_bus();
            }  

            CONFIG::one_wire.convert_temperature(CONFIG::ds18w20_address, true, true);
            #if CONFIG_DS18W20_SAVE_ADDRESS
                WAR_ZONE::dst18w20_data_write = true;
            #endif
            for (int i = 0; i < CONFIG_ONE_WIRE_EXPECTED_AMOUNT_OF_DEVICES; i++) {
                if (i >= CONFIG::ds18w20_amountOfDevices) {
                    WAR_ZONE::dst18w20_temp[i] = -60;
                    continue;
                }
                CONFIG::ds18w20_address = CONFIG::one_wire.get_address(i);
                #if CONFIG_DS18W20_SAVE_ADDRESS
                    WAR_ZONE::dst18w20_lower_address[i] = (CONFIG::ds18w20_address.rom[0] << 24) | (CONFIG::ds18w20_address.rom[1] << 16) | (CONFIG::ds18w20_address.rom[2] << 8) | CONFIG::ds18w20_address.rom[3]; 
                    WAR_ZONE::dst18w20_upper_address[i] = (CONFIG::ds18w20_address.rom[4] << 24) | (CONFIG::ds18w20_address.rom[5] << 16) | (CONFIG::ds18w20_address.rom[6] << 8) | CONFIG::ds18w20_address.rom[7];
                #endif
                WAR_ZONE::dst18w20_temp[i] = CONFIG::one_wire.temperature(CONFIG::ds18w20_address);
                #if DS18W20_PRINTS
                    printf("%i Temperature: %3.5foC\n",i, WAR_ZONE::dst18w20_temp[i]);
                #endif
                
                
            }
            #if CONFIG_DS18W20_SAVE_ADDRESS
                WAR_ZONE::dst18w20_data_write = false;
            #endif
        }

    #endif
}
#endif