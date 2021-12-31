#pragma once
#include <main.h>
#include <helper.h>

class SSD1306_I2C {
public:
    static const size_t width = 128;
    static const size_t height = 64;

private:
    DISALLOW_COPY_AND_ASSIGN(SSD1306_I2C);
    I2C_HandleTypeDef* i2c;
    void ssd1306_command(uint8_t byte);
    void ssd1306_command(uint8_t command, uint8_t arg);
    void ssd1306_data(uint8_t* buffer, size_t buff_size);

    static const uint8_t ssd1306_addr = 0x3C << 1;
    static const size_t ssd1306_buffer_size = width * height / 8;
    uint8_t ssd1306_buffer[ssd1306_buffer_size];

public:
    enum class Color : uint8_t { Black = 0x00, White = 0x01 };

    SSD1306_I2C(I2C_HandleTypeDef* i2c);
    ~SSD1306_I2C();

    void start(void);
    void fill(Color color);
    void flush(void);
    bool flush_completed(void);

    void set_pixel(int32_t x, int32_t y, Color color);
    void set_brightness(uint8_t value);
    void set_display_on(bool on);

    void mirror(bool vertical, bool horizontal);
    void invert(bool inverse);
};
