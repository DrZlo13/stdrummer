#pragma once
#include <stdint.h>
#include <helper.h>
#include <main.h>
#include <math.h>

class HalI2S {
private:
    DISALLOW_COPY_AND_ASSIGN(HalI2S);
    I2S_HandleTypeDef* i2s;
    static const size_t buffer_size = 256 * 4;
    int16_t buffer[buffer_size];

public:
    HalI2S(I2S_HandleTypeDef* i2s);
    ~HalI2S();

    void start();

    typedef void (*Callback)(void* context);
    void set_complete_callback(Callback cb, void* ctx);
    void set_half_complete_callback(Callback cb, void* ctx);

    static constexpr size_t get_buffer_half_size(void) {
        return (buffer_size / 2) * sizeof(int16_t);
    }

    int16_t* get_buffer_first_half(void) {
        return buffer;
    };

    int16_t* get_buffer_second_half(void) {
        return &buffer[buffer_size / 2];
    };
};
