#pragma once
#include <stdint.h>
#include "helper.h"
#include <main.h>

class HalI2S {
private:
    DISALLOW_COPY_AND_ASSIGN(HalI2S);
    I2S_HandleTypeDef* i2s;
    static const size_t buffer_size = 1024;
    int16_t buffer[buffer_size];

public:
    HalI2S(I2S_HandleTypeDef* i2s);
    ~HalI2S();

    void start();
};
