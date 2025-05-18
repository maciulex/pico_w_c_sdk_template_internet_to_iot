#ifndef DISPLAY_LIB_F
#define DISPLAY_LIB_F

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../../config.cpp"

#ifndef I2C_PROTOCOL_DEFINITION
    #define I2C_PROTOCOL_DEFINITION;
    #define I2C_PORT i2c0
    #define I2C_PIN_SDA 12
    #define I2C_PIN_SCL 13
    #define I2C_CLOCK 50*1000

    struct i2c_protocol_t {
        i2c_inst_t * port = I2C_PORT;
        uint8_t      scl = I2C_PIN_SCL;
        uint8_t      sda = I2C_PIN_SDA;
        uint16_t     speed = I2C_CLOCK;

        bool         initialized = false;
        /* data */
    };
#endif


namespace LCD_API {
    i2c_protocol_t* i2c;
    uint8_t active_adresss = 0;

    uint8_t address [CONFIG_AMOUNT_OF_DISPLAYS];
    uint8_t params  [CONFIG_AMOUNT_OF_DISPLAYS][2];//[0] lines [1] columns
    uint8_t position[CONFIG_AMOUNT_OF_DISPLAYS][2];//[0] x [1] y
    bool implied_newline[CONFIG_AMOUNT_OF_DISPLAYS];


    bool backlight;


    uint8_t LCD_CLR = 0x01;        //DB0: clear display
    uint8_t LCD_HOM = 0X02;        //DB1: return to home position

    uint8_t LCD_ENTRY_MODE = 0x04; //DB2: set entry mode
    uint8_t LCD_ENTRY_INC  = 0x02; //--DB1: increment
    uint8_t LCD_ENTRY_SHIFT= 0x01; //--DB0: shift

    uint8_t LCD_ON_CTRL    = 0x08; //DB3: turn lcd/cursor on
    uint8_t LCD_ON_DISPLAY = 0x04; //--DB2: turn display on
    uint8_t LCD_ON_CURSOR  = 0x02; //--DB1: turn cursor on
    uint8_t LCD_ON_BLINK   = 0x01; //--DB0: blinking cursor

    uint8_t LCD_MOVE_      = 0x10; //DB4: move cursor/display
    uint8_t LCD_MOVE_DISP  = 0x08; //--DB3: move display (0-> move cursor)
    uint8_t LCD_MOVE_RIGHT = 0x04; //--DB2: move right (0-> left)

    uint8_t LCD_FUNCTION       = 0x20; //DB5: function set
    uint8_t LCD_FUNCTION_8BIT  = 0x10; //--DB4: set 8BIT mode (0->4BIT mode)
    uint8_t LCD_FUNCTION_2LINE = 0x08; //--DB3: two lines (0->one line)
    uint8_t LCD_FUNCTION_10DOTS= 0x04; //--DB2: 5x10 font (0->5x7 font)
    uint8_t LCD_FUNCTION_RESET = 0x30; //See "Initializing by Instruction" section

    uint8_t LCD_CGRAM = 0x40;           //# DB6: set CG RAM address
    uint8_t LCD_DDRAM = 0x80;           //# DB7: set DD RAM address
    
    uint8_t LCD_RS_CMD  = 0;
    uint8_t LCD_RS_DATA = 1;
    
    uint8_t LCD_RW_WRITE = 0;
    uint8_t LCD_RW_READ  = 1;

    uint8_t SHIFT_DATA        = 4;
    uint8_t SHIFT_BACKLIGHT   = 3;
    uint8_t MASK_RS = 0x01;
    uint8_t MASK_RW = 0x02;
    uint8_t MASK_E  = 0x04;
    
    void hal_write_command(uint8_t a, uint8_t cmd);
    void hal_write_data(uint8_t a, uint8_t data);

    void clear(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        hal_write_command(a, LCD_CLR);
        hal_write_command(a, LCD_HOM);
        position[i][0] = 0;
        position[i][1] = 0;
    }

    void show_cursor(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        uint8_t cmd = LCD_ON_CTRL | LCD_ON_DISPLAY | LCD_ON_CURSOR;
        hal_write_command(a, cmd);
    }

    void hide_cursor(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        uint8_t cmd = LCD_ON_CTRL | LCD_ON_DISPLAY;
        hal_write_command(a, cmd);
    }

    void blink_cursor(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        uint8_t cmd = LCD_ON_CTRL | LCD_ON_DISPLAY | LCD_ON_CURSOR | LCD_ON_BLINK;
        hal_write_command(a, cmd);
    }

    void blink_cursor_off(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        uint8_t cmd = LCD_ON_CTRL | LCD_ON_DISPLAY | LCD_ON_CURSOR;
        hal_write_command(a, cmd);
    }

    void display_on(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        uint8_t cmd = LCD_ON_CTRL | LCD_ON_DISPLAY;
        hal_write_command(a, cmd);
    }

    void display_off(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        uint8_t cmd = LCD_ON_CTRL;
        hal_write_command(a, cmd);
    }

    void backlight_on(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];

        uint8_t b = 1 << SHIFT_BACKLIGHT;
        i2c_write_blocking(i2c->port, a, &b, 1, false);
        backlight = true;
    }

    void backlight_off(uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        uint8_t b = 0;
        i2c_write_blocking(i2c->port, a, &b, 1, false);
        backlight = false;
    }

    void move_to(uint8_t x = 0, uint8_t y = 0, uint8_t i = 255) {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];
        position[i][0] = x;
        position[i][1] = y;

        uint8_t dis_addr = x & 0x3f;
        if (y & 1) dis_addr += 0x40;
        if (y & 2) dis_addr += params[i][1];
        dis_addr |= LCD_DDRAM;
        hal_write_command(a, dis_addr);
    }

    void put_char(uint8_t i = 255, char x = ' ') {
        uint8_t a;
        if (i == 255) {
            a = address[active_adresss];
            i = active_adresss;
        } else a = address[i];

        printf("old pos: x: %i y: %i \n", position[i][0], position[i][1]);
        if (x == '\n') {
            if (implied_newline[i]) {
                implied_newline[i] = false;
            } else {
                position[i][0] = params[i][1];
            }
        } else {
            printf("adding one to x\n");
            hal_write_data(a, (uint8_t)x);
            position[i][0] += 1;
        }
        if (position[i][0] >= params[i][1]) {
            position[i][0]  = 0;
            position[i][1] += 1;
            implied_newline[i] = ((x != '\n') ? true : false);
        }
        if (position[i][1] >= params[i][0]) {
            position[i][1] = 0;
        }
        move_to(position[i][0], position[i][1], i);
        printf("new pos: x: %i y: %i \n", position[i][0], position[i][1]);

    }

    void put_str(std::string s, uint8_t index = 255) {
        if (index == 255) index = active_adresss;
        for (int i = 0; i < s.length(); i++) {
            put_char(index,s[i]);
        }
    }

    bool set_device_by_address(uint8_t a) {
        for (uint8_t i = 0; i < CONFIG_AMOUNT_OF_DISPLAYS; i++) {
            if (address[i] == a) {
                active_adresss = i;
                return true;
            }
        }
        return false;
    }

    bool set_device_by_index(uint8_t i) {
        if (i >= CONFIG_AMOUNT_OF_DISPLAYS) return false;
        if (address[i] == 0) return false;
        active_adresss = i;
        return true;
    }

    bool add_display_address(uint8_t a, uint8_t num_lines, uint8_t num_columns) {
        for (uint8_t i = 0; i < CONFIG_AMOUNT_OF_DISPLAYS; i++) {
            if (address[i] == 0) {
                address[i] = a;
                params[i][0] = num_lines;
                params[i][1] = num_columns;
                return true;
            }
        }
        return false;
    }

    void hal_write_init_nibble(uint8_t a, uint8_t nibble) {
        uint8_t byte = ((nibble >> 4) & 0x0f) << SHIFT_DATA;
        uint8_t byteE = (((nibble >> 4) & 0x0f) << SHIFT_DATA) | MASK_E;

        i2c_write_blocking(i2c->port, a, &byteE, 1, false);
        i2c_write_blocking(i2c->port, a, &byte,  1, false);

    }

    void hal_write_command(uint8_t a, uint8_t cmd) {
        uint8_t byte  =  ((backlight << SHIFT_BACKLIGHT) | (((cmd >> 4) & 0x0f)<<SHIFT_DATA));
        uint8_t byteE = (((backlight << SHIFT_BACKLIGHT) | (((cmd >> 4) & 0x0f)<<SHIFT_DATA))) | MASK_E;
        i2c_write_blocking(i2c->port, a, &byteE, 1, false);
        i2c_write_blocking(i2c->port, a, &byte,  1, false);
        byte  =  ((backlight << SHIFT_BACKLIGHT) | ((cmd & 0x0f) << SHIFT_DATA));
        byteE = (((backlight << SHIFT_BACKLIGHT) | ((cmd & 0x0f) << SHIFT_DATA))) | MASK_E;
        i2c_write_blocking(i2c->port, a, &byteE, 1, false);
        i2c_write_blocking(i2c->port, a, &byte,  1, false);

        if (cmd <= 3) {
            sleep_ms(5);
        }

    }

    void hal_write_data(uint8_t a, uint8_t data) {
        uint8_t byte  =  (MASK_RS | (backlight << SHIFT_BACKLIGHT) | (((data >> 4) & 0x0f) << SHIFT_DATA));
        uint8_t byteE = ((MASK_RS | (backlight << SHIFT_BACKLIGHT) | (((data >> 4) & 0x0f) << SHIFT_DATA))) | MASK_E;
        i2c_write_blocking(i2c->port, a, &byteE, 1, false);
        i2c_write_blocking(i2c->port, a, &byte,  1, false);

        byte  =  (MASK_RS | (backlight << SHIFT_BACKLIGHT) | ((data & 0x0f) << SHIFT_DATA));
        byteE = ((MASK_RS | (backlight << SHIFT_BACKLIGHT) | ((data & 0x0f) << SHIFT_DATA))) | MASK_E;
        i2c_write_blocking(i2c->port, a, &byteE, 1, false);
        i2c_write_blocking(i2c->port, a, &byte,  1, false);
    }

    void i2cInit() {
        i2c->speed = i2c_init(i2c->port, i2c->speed);
        gpio_set_function(i2c->sda, GPIO_FUNC_I2C);
        gpio_set_function(i2c->scl, GPIO_FUNC_I2C);
        gpio_pull_up(i2c->sda);
        gpio_pull_up(i2c->scl);

        i2c->initialized = true;
    }

    void init_display_param(uint8_t index) {
        printf("init params display %i addr: %i\n", index, address[index]);

        if (params[index][0] > 4 ) params[index][0] = 4;
        if (params[index][1] > 40) params[index][1] = 40;
        position[index][0] = 0;
        position[index][1] = 0;
        implied_newline[index] = false;
        backlight = true;
        display_off(index);
        backlight_on(index);
        clear(index);
        hal_write_command(address[index], LCD_ENTRY_MODE | LCD_ENTRY_INC);
        hide_cursor(index);
        display_on(index);
    }

    void init_display(uint8_t i) {

        printf("init display %i addr: %i\n", i, address[i]);
        uint8_t buff;
        i2c_write_blocking(i2c->port, address[i], &buff, 1, false);
        sleep_ms(20);
        hal_write_init_nibble(address[i],LCD_FUNCTION_RESET);
        sleep_ms(5);
        hal_write_init_nibble(address[i],LCD_FUNCTION_RESET);
        sleep_ms(1);
        hal_write_init_nibble(address[i],LCD_FUNCTION_RESET);
        sleep_ms(1);
        hal_write_init_nibble(address[i],LCD_FUNCTION);
        sleep_ms(1);
        init_display_param(i);
        buff = LCD_FUNCTION;
        if(params[i][0] > 1) {
            buff |= LCD_FUNCTION_2LINE;
        }
        hal_write_command(address[i], buff);

    }

    void init_all_displays() {
        for (uint8_t i = 0; i < CONFIG_AMOUNT_OF_DISPLAYS; i++) {
            if (address[i] == 0) continue;
            init_display(i);
        }
    }

    void init(i2c_protocol_t* i2c_p, bool init_displays = true){
        i2c = i2c_p;
        if (!(i2c->initialized)) {
           i2cInit();
        }

        if (init_displays) {
            init_all_displays();
        }

    }
}

#endif