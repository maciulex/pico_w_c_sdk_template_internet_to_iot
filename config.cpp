#ifndef CONFIG_FILE_F
#define CONFIG_FILE_F

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#include <string>
#include <cstdint>

    #define I2C_PROTOCOL_DEFINITION
    #define I2C_PORT i2c0
    #define I2C_PIN_SDA    12
    #define I2C_PIN_SCL    13
    #define I2C_SPEED 100000

    #define FALSE 0
    #define TRUE  1

    #define CONFIG_ON_CORE_1_STUCK 1 // 1 -> reboot board, 2->reboot only core

    #define INTERNET_ENABLE_AUTO_RAPORT FALSE //Yet to be implemented

    #define CONFIG_HARMONOGRAM_ENABLED FALSE  //Yet to be implemented
    #define CONFIG_DISPLAY_ENABLED     TRUE   
    #define CONFIG_DHT20_ENABLED       FALSE  //Yet to be implemented, dont have sensor to test it out
    #define CONFIG_BMP280_ENABLED      FALSE  
    #define CONFIG_DS18W20_ENABLED     FALSE  

    #define CONFIG_ONE_WIRE_PIN                        15
    #define CONFIG_ONE_WIRE_EXPECTED_AMOUNT_OF_DEVICES 1
    #define CONFIG_DS18W20_SAVE_ADDRESS                TRUE //In data communication file will be 2x32bit arr for address of device

    #define CONFIG_AMOUNT_OF_DISPLAYS 1

    //PRINTS

    #define CORE0_PRINT_HEARTBEAT           FALSE
    #define CORE1_PRINT_HEARTBEAT           TRUE

    #define INTERNET_PRINT_HTML_RESPONSE    TRUE
    #define INTERNET_PRINT_DEBUG            TRUE

    #define DS18W20_PRINTS                  TRUE
    #define CONFIG_BMP280_PRINT_AFTER_READ  TRUE

    struct i2c_protocol_t {
        i2c_inst_t * port;
        uint8_t      scl;
        uint8_t      sda;
        uint16_t     speed;

        bool         initialized = false;
        /* data */
    };

    #if CONFIG_DS18W20_ENABLED
        #include "core0/libs/build_in/one_wire/api/one_wire.h"
    #endif
    
    
    namespace CONFIG {

        const uint8_t          PICO_ID = 7;
        const char*          PICO_DESC = "c_test_dev";
        std::string       PICO_VERSION = "0.74";
        std::string PICO_REQUEST_HEADER= "?pico_id="+std::to_string(PICO_ID)+"&pico_desc="+PICO_DESC;    

        std::string SERVER_MAIN_PATH = "/newSystem/traffic_control.php";
        std::string SERVER_IP        = "192.168.1.2";
        uint16_t    SERVER_PORT      = 80;

        bool TIME_INITIETED = false;

        bool config_hight_button        = false;

        i2c_protocol_t i2c;

        uint8_t REPORT_TIME = 1;//MINS
        uint8_t PING_TIME   = 1;//MINS

        
        #if CONFIG_DS18W20_ENABLED
            One_wire one_wire(CONFIG_ONE_WIRE_PIN); 
            uint8_t ds18w20_amountOfDevices;
            rom_address_t ds18w20_address{};
        #endif

        void init_i2c0() {
            i2c.port = I2C_PORT;
            i2c.scl  = I2C_PIN_SCL;
            i2c.sda  = I2C_PIN_SDA;

            i2c.speed = i2c_init(i2c.port, I2C_SPEED);
            i2c.initialized = true;

            gpio_set_function(i2c.sda, GPIO_FUNC_I2C);
            gpio_set_function(i2c.scl, GPIO_FUNC_I2C);
            gpio_pull_up(i2c.sda);
            gpio_pull_up(i2c.scl); 
        }
        
        void init() {
            #if (CONFIG_DISPLAY_ENABLED) || (CONFIG_DHT20_ENABLED) || (CONFIG_BMP280_ENABLED)
                init_i2c0();
            #endif
    
        }




    }


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
#endif
