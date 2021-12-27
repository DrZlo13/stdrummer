#include <main.h>
#include <core.h>
#include <string.h>
#include "ssd1306-i2c.h"

void SSD1306_I2C::ssd1306_command(uint8_t command) {
    HAL_I2C_Mem_Write(this->i2c, ssd1306_addr, 0x00, 1, &command, 1, HAL_MAX_DELAY);
}

void SSD1306_I2C::ssd1306_data(uint8_t* buffer, size_t buff_size) {
    HAL_I2C_Mem_Write_DMA(this->i2c, ssd1306_addr, 0x40, 1, buffer, buff_size);
}

void SSD1306_I2C::set_brightness(uint8_t value) {
    ssd1306_command(0x81);
    ssd1306_command(value);
}

void SSD1306_I2C::fill(Color color) {
    memset(ssd1306_buffer, (color == Color::Black) ? 0x00 : 0xFF, ssd1306_buffer_size);
}

void SSD1306_I2C::flush(void) {
    // TODO back buffer
    // set page start address for page addressing mode to 0
    ssd1306_command(0xB0);
    ssd1306_command(0x00);
    ssd1306_command(0x10);
    ssd1306_data(ssd1306_buffer, ssd1306_buffer_size);
}

bool SSD1306_I2C::flush_completed() {
    return HAL_I2C_GetState(this->i2c) == HAL_I2C_STATE_READY;
}

void SSD1306_I2C::set_pixel(int32_t x, int32_t y, Color color) {
    if(x < 0 || y < 0) {
        return;
    }

    if((size_t)x >= width || (size_t)y >= height) {
        return;
    }

    if(color == Color::White) {
        ssd1306_buffer[x + (y / 8) * width] |= 1 << (y % 8);
    } else {
        ssd1306_buffer[x + (y / 8) * width] &= ~(1 << (y % 8));
    }
}

void SSD1306_I2C::set_display_on(bool on) {
    if(on) {
        // Display on
        ssd1306_command(0xAF);
    } else {
        // Display off
        ssd1306_command(0xAE);
    }
}

SSD1306_I2C::SSD1306_I2C(I2C_HandleTypeDef* i2c) {
    this->i2c = i2c;
}

SSD1306_I2C::~SSD1306_I2C() {
}

void SSD1306_I2C::start(bool mirror_v, bool mirror_h, bool inverse) {
    // Wait for the screen to boot
    HAL_Delay(100);

    // Display off
    set_display_on(false);

    //Set Memory Addressing Mode
    ssd1306_command(0x20);

    // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
    // 10b,Page Addressing Mode (RESET); 11b,Invalid
    ssd1306_command(0x00);

    //Set Page Start Address for Page Addressing Mode,0-7
    ssd1306_command(0xB0);

    if(mirror_v) {
        // Mirror vertically
        ssd1306_command(0xC0);
    } else {
        //Set COM Output Scan Direction
        ssd1306_command(0xC8);
    }

    ssd1306_command(0x00); //---set low column address
    ssd1306_command(0x10); //---set high column address
    ssd1306_command(0x40); //--set start line address - CHECK

    set_brightness(0xFF);

    if(mirror_h) {
        // Mirror horizontally
        ssd1306_command(0xA0);
    } else {
        //--set segment re-map 0 to 127 - CHECK
        ssd1306_command(0xA1);
    }

    if(inverse) {
        // set inverse color
        ssd1306_command(0xA7);
    } else {
        // set normal color
        ssd1306_command(0xA6);
    }

    // Set multiplex ratio.
    if(height == 128) {
        // Found in the Luma Python lib for SH1106.
        ssd1306_command(0xFF);
    } else {
        // set multiplex ratio(1 to 64) - CHECK
        ssd1306_command(0xA8);
    }

    switch(height) {
    case 32:
        ssd1306_command(0x1F);
        break;
    case 64:
        ssd1306_command(0x3F);
        break;
    case 128:
        // Seems to work for 128px high displays too.
        ssd1306_command(0x3F);
        break;
    default:
        core_crash("Only 32, 64, or 128 lines of height are supported!");
        break;
    }

    // 0xA4 = Output follows RAM content; 0xA5 = Output ignores RAM content
    ssd1306_command(0xA4);

    // set display offset - CHECK
    ssd1306_command(0xD3);
    // not offset
    ssd1306_command(0x00);

    ssd1306_command(0xD5); //--set display clock divide ratio/oscillator frequency
    ssd1306_command(0xF0); //--set divide ratio

    ssd1306_command(0xD9); //--set pre-charge period
    ssd1306_command(0x22); //

    ssd1306_command(0xDA); //--set com pins hardware configuration - CHECK

    switch(height) {
    case 32:
        ssd1306_command(0x02);
        break;
    case 64:
        ssd1306_command(0x12);
        break;
    case 128:
        ssd1306_command(0x12);
        break;
    default:
        core_crash("Only 32, 64, or 128 lines of height are supported!");
        break;
    }

    ssd1306_command(0xDB); //--set vcomh
    ssd1306_command(0x20); //0x20,0.77xVcc

    ssd1306_command(0x8D); //--set DC-DC enable
    ssd1306_command(0x14); //
    set_display_on(true);

    // Clear screen
    fill(Color::White);

    // Flush buffer to screen
    flush();
}