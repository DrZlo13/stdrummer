#include "hal-i2s.h"
#include <iterator>
#include <algorithm>
#include "../hal.h"

#include <math.h>

extern DMA_HandleTypeDef hdma_spi1_tx;

HalI2S::HalI2S(I2S_HandleTypeDef* i2s) {
    this->i2s = i2s;
    const float pi = 3.14159265f;
    for(size_t i = 0; i < this->buffer_size; i++) {
        // saw
        //buffer[i] = (i / 2) * (UINT_LEAST16_MAX / 512) - UINT_LEAST16_MAX / 2;

        // sine
        buffer[i] = sin(i * (pi / 512)) * (UINT_LEAST16_MAX / 2);
    }
}

HalI2S::~HalI2S() {
}

void HalI2S::start() {
    uint16_t* data = reinterpret_cast<uint16_t*>(this->buffer);
    HAL_I2S_Transmit_DMA(this->i2s, data, this->buffer_size);
}
