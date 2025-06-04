#ifndef CONFIG_FILE_F
#define CONFIG_FILE_F

#define FALSE 0
#define TRUE  1

#include "pico/stdlib.h"
#include <time.h>   
#include <string>
#include <cstdint>
    #define USER_DECLARE_I2C  FALSE
    #define USER_DECLARE_UART FALSE


    #define CONFIG_ON_CORE_1_STUCK 1 // 1 -> reboot board, 2->reboot only core

    #define INTERNET_ENABLE_AUTO_RAPORT TRUE //Yet to be implemented

    #define CONFIG_HARMONOGRAM_ENABLED FALSE  //Yet to be implemented
    #define CONFIG_DISPLAY_ENABLED     TRUE   
    #define CONFIG_DHT20_ENABLED       FALSE  //Yet to be implemented, dont have sensor to test it out
    #define CONFIG_BMP280_ENABLED      FALSE  
    #define CONFIG_DS18W20_ENABLED     TRUE  
    #define CONFIG_PZEM004_ENABLED     TRUE

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
    
    #define I2C_ENABLE  (USER_DECLARE_I2C  || CONFIG_DISPLAY_ENABLED || CONFIG_DHT20_ENABLED || CONFIG_BMP280_ENABLED)
    #define UART_ENABLE (USER_DECLARE_UART || CONFIG_PZEM004_ENABLED)

    #if I2C_ENABLE
        #define I2C_PROTOCOL_DEFINITION
        #include "hardware/i2c.h"
        #define I2C_PORT i2c0
        #define I2C_PIN_SDA    12
        #define I2C_PIN_SCL    13
        #define I2C_SPEED 100000
    #endif
    #if UART_ENABLE
        #define UART_PROTOCOL_DEFINITION
        #include "hardware/uart.h"
        #define UART_PORT uart0
        #define UART_PIN_TX    0
        #define UART_PIN_RX    1
        #define UART_SPEED 9600
    #endif

    #if I2C_ENABLE
        struct i2c_protocol_t {
            i2c_inst_t * port;
            uint8_t      scl;
            uint8_t      sda;
            uint16_t     speed;

            bool         initialized = false;
            /* data */
        };
    #endif
    #if UART_ENABLE
        struct uart_protocol_t {
            uart_inst_t * port;
            uint8_t      tx;
            uint8_t      rx;
            uint16_t     speed;

            bool         initialized = false;
            /* data */
        };
    #endif
    #if CONFIG_DS18W20_ENABLED
        #include "user/core0/libs/build_in/one_wire/api/one_wire.h"
    #endif
    
    
    namespace CONFIG {

        const uint8_t          PICO_ID = 7;
        const char*          PICO_DESC = "c_test_dev";
        std::string       PICO_VERSION = "0.75";
        std::string PICO_REQUEST_HEADER= "?pico_id="+std::to_string(PICO_ID)+"&pico_desc="+PICO_DESC;    

        std::string SERVER_MAIN_PATH = "/newSystem/traffic_control.php";
        std::string SERVER_IP        = "192.168.1.2";
        uint16_t    SERVER_PORT      = 80;

        bool TIME_INITIETED = false;

        bool config_hight_button        = false;

        uint8_t REPORT_TIME = 1;//MINS
        uint8_t PING_TIME   = 1;//MINS

        time_t core1_last_activity = 0;
        
        #if CONFIG_DS18W20_ENABLED
            One_wire one_wire(CONFIG_ONE_WIRE_PIN); 
            uint8_t ds18w20_amountOfDevices;
            rom_address_t ds18w20_address{};
        #endif
        #if (UART_ENABLE)
            uart_protocol_t uart;
            void init_uart() {
                uart.port = UART_PORT;
                uart.tx   = UART_PIN_TX;
                uart.rx   = UART_PIN_RX;
                uart.initialized = true;

                uart.speed = uart_init(uart.port, UART_SPEED);
                gpio_set_function(uart.tx, GPIO_FUNC_UART);
                gpio_set_function(uart.rx, GPIO_FUNC_UART);
                uart_set_format(uart.port, 8, 1, UART_PARITY_NONE);
            }
        #endif            
        #if (I2C_ENABLE)
            i2c_protocol_t  i2c;
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
        #endif

        void init() {
            #if (I2C_ENABLE)
                init_i2c0();
            #endif
            #if (UART_ENABLE)
                init_uart();
            #endif
            datetime_t t = {
                .year = 2025,
                .month = 1,
                .day = 1,
                .dotw = 1, // day of the week (1 = Monday)
                .hour = 1,
                .min = 1,
                .sec = 1
            };
            rtc_set_datetime(&t);
        }




    }

    time_t get_mktime(datetime_t * t) {
        struct tm tm_time;
        tm_time.tm_sec =  t->sec;
        tm_time.tm_min =  t->min;
        tm_time.tm_hour = t->hour;
        tm_time.tm_mday = t->day;
        tm_time.tm_mon =  t->month - 1;      // struct tm: 0–11
        tm_time.tm_year = t->year - 1900;   // struct tm: lata od 1900
        tm_time.tm_isdst = -1; 
        return mktime(&tm_time); 
    }
    void core1_watch_dog() {
        datetime_t time_now;
        rtc_get_datetime(&time_now);
        CONFIG::core1_last_activity = get_mktime(&time_now);
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
