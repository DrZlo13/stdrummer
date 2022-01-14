#pragma once
#include <main.h>
#include <helper.h>

template <size_t width_sz, size_t height_sz> class SSD1306_I2C {
private:
    DISALLOW_COPY_AND_ASSIGN(SSD1306_I2C);
    I2C_HandleTypeDef* i2c;
    void ssd1306_command(uint8_t byte);
    void ssd1306_command(uint8_t command, uint8_t arg);
    void ssd1306_data(uint8_t* buffer, size_t buff_size);

    static const uint8_t ssd1306_addr = 0x3C << 1;
    static const size_t ssd1306_buffer_size = width_sz * height_sz / 8;
    uint8_t ssd1306_buffer[ssd1306_buffer_size];

    typedef enum {
        SetColumnLowNibble0 = 0x00,
        SetColumnLowNibble1 = 0x01,
        SetColumnLowNibble2 = 0x02,
        SetColumnLowNibble3 = 0x03,
        SetColumnLowNibble4 = 0x04,
        SetColumnLowNibble5 = 0x05,
        SetColumnLowNibble6 = 0x06,
        SetColumnLowNibble7 = 0x07,
        SetColumnLowNibble8 = 0x08,
        SetColumnLowNibble9 = 0x09,
        SetColumnLowNibbleA = 0x0A,
        SetColumnLowNibbleB = 0x0B,
        SetColumnLowNibbleC = 0x0C,
        SetColumnLowNibbleD = 0x0D,
        SetColumnLowNibbleE = 0x0E,
        SetColumnLowNibbleF = 0x0F,
        SetColumnHighNibble0 = 0x10,
        SetColumnHighNibble1 = 0x11,
        SetColumnHighNibble2 = 0x12,
        SetColumnHighNibble3 = 0x13,
        SetColumnHighNibble4 = 0x14,
        SetColumnHighNibble5 = 0x15,
        SetColumnHighNibble6 = 0x16,
        SetColumnHighNibble7 = 0x17,
        SetColumnHighNibble8 = 0x18,
        SetColumnHighNibble9 = 0x19,
        SetColumnHighNibbleA = 0x1A,
        SetColumnHighNibbleB = 0x1B,
        SetColumnHighNibbleC = 0x1C,
        SetColumnHighNibbleD = 0x1D,
        SetColumnHighNibbleE = 0x1E,
        SetColumnHighNibbleF = 0x1F,
        SetMemoryMode = 0x20,
        SetColumnAddress = 0x21,
        SetPageAddress = 0x22,
        SetStartLine = 0x40,
        SetContrast = 0x81,
        SetChargePump = 0x8D,
        SetPageStartAddress = 0xB0,
        SetSegmentRemapOn = 0xA0,
        SetSegmentRemapOff = 0xA1,
        AllowOnResume = 0xA4,
        AllowOn = 0xA5,
        InvertDisplayOff = 0xA6,
        InvertDisplayOn = 0xA7,
        SetMultiplexRatio = 0xA8,
        DisplayOff = 0xAE,
        DisplayOn = 0xAF,
        ComScanIncrease = 0xC0,
        ComScanDecrease = 0xC8,
        SetOffset = 0xD3,
        SetClockDiv = 0xD5,
        SetPrecharge = 0xD9,
        SetComPins = 0xDA,
        SetVcomDetect = 0xDB,
    } Cmd;

public:
    const size_t width = width_sz;
    const size_t height = height_sz;

    enum class Color : uint8_t { Black = 0x00, White = 0x01 };

    SSD1306_I2C(I2C_HandleTypeDef* i2c);
    ~SSD1306_I2C();

    void start();

    void fill(Color color);
    void flush(void);
    bool flush_completed(void);

    void set_pixel(int32_t x, int32_t y, Color color);
    void set_brightness(uint8_t value);
    void set_display_on(bool on);

    void mirror(bool vertical, bool horizontal);
    void invert(bool inverse);
};

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::ssd1306_command(uint8_t command) {
    HAL_I2C_Mem_Write(this->i2c, ssd1306_addr, 0x00, 1, &command, 1, HAL_MAX_DELAY);
}

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::ssd1306_command(uint8_t command, uint8_t arg) {
    uint8_t data[2] = {command, arg};
    HAL_I2C_Mem_Write(this->i2c, ssd1306_addr, 0x00, 1, data, 2, HAL_MAX_DELAY);
}

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::ssd1306_data(uint8_t* buffer, size_t buff_size) {
    HAL_I2C_Mem_Write_DMA(this->i2c, ssd1306_addr, 0x40, 1, buffer, buff_size);
}

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::set_brightness(uint8_t value) {
    ssd1306_command(Cmd::SetContrast, value);
}

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::fill(Color color) {
    memset(ssd1306_buffer, (color == Color::Black) ? 0x00 : 0xFF, ssd1306_buffer_size);
}

template <size_t width_sz, size_t height_sz> void SSD1306_I2C<width_sz, height_sz>::flush(void) {
    // TODO back buffer
    if constexpr(((width_sz == 64) && (height_sz == 48)) || ((width_sz == 64) && (height_sz == 32))) {
        for(uint8_t i = 0; i < (height_sz / 8); i++) {
            ssd1306_command(Cmd::SetPageStartAddress | i);
            ssd1306_command(Cmd::SetColumnLowNibble0);
            ssd1306_command(Cmd::SetColumnHighNibble2);
            //(uint8_t *)&screenmemory[i * 0x40], 0x40

            ssd1306_data(&ssd1306_buffer[i * width_sz], width_sz);
            while(!flush_completed()) {
            }
        }
    } else {
        // set page start address to 0x00
        ssd1306_command(Cmd::SetPageStartAddress);
        ssd1306_command(Cmd::SetColumnLowNibble0);
        ssd1306_command(Cmd::SetColumnHighNibble0);
        ssd1306_data(ssd1306_buffer, ssd1306_buffer_size);
    }
}

template <size_t width_sz, size_t height_sz>
bool SSD1306_I2C<width_sz, height_sz>::flush_completed() {
    return HAL_I2C_GetState(this->i2c) == HAL_I2C_STATE_READY;
}

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::set_pixel(int32_t x, int32_t y, Color color) {
    if(x < 0 || y < 0) {
        return;
    }

    if((size_t)x >= width_sz || (size_t)y >= height_sz) {
        return;
    }

    if(color == Color::White) {
        ssd1306_buffer[x + (y / 8) * width_sz] |= 1 << (y % 8);
    } else {
        ssd1306_buffer[x + (y / 8) * width_sz] &= ~(1 << (y % 8));
    }
}

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::set_display_on(bool on) {
    if(on) {
        // Display on
        ssd1306_command(Cmd::DisplayOn);
    } else {
        // Display off
        ssd1306_command(Cmd::DisplayOff);
    }
}

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::mirror(bool vertical, bool horizontal) {
    if(vertical) {
        ssd1306_command(Cmd::ComScanIncrease);
    } else {
        ssd1306_command(Cmd::ComScanDecrease);
    }

    if(horizontal) {
        ssd1306_command(Cmd::SetSegmentRemapOn);
    } else {
        ssd1306_command(Cmd::SetSegmentRemapOff);
    }
}

template <size_t width_sz, size_t height_sz>
void SSD1306_I2C<width_sz, height_sz>::invert(bool inverse) {
    if(inverse) {
        ssd1306_command(Cmd::InvertDisplayOn);
    } else {
        ssd1306_command(Cmd::InvertDisplayOff);
    }
}

template <size_t width_sz, size_t height_sz>
SSD1306_I2C<width_sz, height_sz>::SSD1306_I2C(I2C_HandleTypeDef* i2c) {
    this->i2c = i2c;
}

template <size_t width_sz, size_t height_sz> SSD1306_I2C<width_sz, height_sz>::~SSD1306_I2C() {
}

template <size_t width_sz, size_t height_sz> void SSD1306_I2C<width_sz, height_sz>::start() {
    const bool external_voltage_supplied = false;

    ssd1306_command(Cmd::DisplayOff);
    ssd1306_command(Cmd::SetClockDiv, 0x80);
    ssd1306_command(Cmd::SetMultiplexRatio, height_sz - 1);

    ssd1306_command(Cmd::SetOffset, 0x00);
    ssd1306_command(Cmd::SetStartLine | 0x0);
    if(external_voltage_supplied) {
        // display voltage supplied externally
        ssd1306_command(Cmd::SetChargePump, 0x10);
    } else {
        // display voltage generated by charge pump from vcc
        ssd1306_command(Cmd::SetChargePump, 0x14);
    }

    ssd1306_command(Cmd::SetMemoryMode, 0x00);
    ssd1306_command(Cmd::SetSegmentRemapOff);
    ssd1306_command(Cmd::ComScanDecrease);

    if constexpr((width_sz == 128) && (height_sz == 32)) {
        ssd1306_command(Cmd::SetComPins, 0x02);
        ssd1306_command(Cmd::SetContrast, 0x8F);
    } else if constexpr((width_sz == 128) && (height_sz == 64)) {
        ssd1306_command(Cmd::SetComPins, 0x12);
        ssd1306_command(Cmd::SetContrast, external_voltage_supplied ? 0x9F : 0xCF);
    } else if constexpr((width_sz == 96) && (height_sz == 16)) {
        ssd1306_command(Cmd::SetComPins, 0x02);
        ssd1306_command(Cmd::SetContrast, external_voltage_supplied ? 0x10 : 0xAF);
    } else if constexpr((width_sz == 64) && (height_sz == 48)) {
        ssd1306_command(Cmd::SetComPins, 0x12);
        ssd1306_command(Cmd::SetContrast, 0x8F);
    } else if constexpr((width_sz == 64) && (height_sz == 32)) {
        // TODO: find info about SetComPins
        ssd1306_command(Cmd::SetComPins, 0x12);
        ssd1306_command(Cmd::SetContrast, 0x8F);
    } else {
        Core::crash("Unknown display");
    }

    ssd1306_command(Cmd::SetPrecharge, external_voltage_supplied ? 0x22 : 0xF1);
    ssd1306_command(Cmd::SetVcomDetect, 0x40);
    ssd1306_command(Cmd::AllowOnResume);
    ssd1306_command(Cmd::InvertDisplayOff);
    ssd1306_command(Cmd::DisplayOn);

    fill(Color::Black);
    flush();
    while(!flush_completed()) {
    }
}