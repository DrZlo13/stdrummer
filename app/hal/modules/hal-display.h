#pragma once
#include <main.h>
#include "helper.h"

class HalDisplay {
private:
    DISALLOW_COPY_AND_ASSIGN(HalDisplay);
    I2C_HandleTypeDef* i2c;
    void ssd1306_command(uint8_t byte);
    void ssd1306_data(uint8_t* buffer, size_t buff_size);
    void ssd1306_set_contrast(const uint8_t value);
    void ssd1306_set_display_on(const uint8_t on);

    static const uint8_t ssd1306_addr = 0x3C << 1;
    static const size_t ssd1306_width = 128;
    static const size_t ssd1306_height = 64;
    static const size_t ssd1306_buffer_size = ssd1306_width * ssd1306_height / 8;
    uint8_t ssd1306_buffer[ssd1306_buffer_size];

public:
    HalDisplay(I2C_HandleTypeDef* i2c);
    ~HalDisplay();
    void start(bool mirror_v, bool mirror_h, bool inverse);

    enum class Color : uint8_t { Black = 0x00, White = 0x01 };

    void fill(Color color);
    void flush(void);
};
