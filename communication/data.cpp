#ifndef COMMUNICATION_DATA_F
#define COMMUNICATION_DATA_F

#include "../config.cpp"

namespace WAR_ZONE {

    //things that are shared between cores
    //should be 32bit wide or rediness flag implemented
    //to avoid reading when other core is writing

    #if CONFIG_DS18W20_ENABLED
        float dst18w20_temp[CONFIG_ONE_WIRE_EXPECTED_AMOUNT_OF_DEVICES];
        #if CONFIG_DS18W20_SAVE_ADDRESS
            bool dst18w20_data_write = false;
            uint32_t dst18w20_lower_address[CONFIG_ONE_WIRE_EXPECTED_AMOUNT_OF_DEVICES];
            uint32_t dst18w20_upper_address[CONFIG_ONE_WIRE_EXPECTED_AMOUNT_OF_DEVICES];
        #endif

    #endif

    #if CONFIG_BMP280_ENABLED
        int32_t bmp280_temp = -60;
        int32_t bmp280_press= -60;
    #endif

} 
#endif