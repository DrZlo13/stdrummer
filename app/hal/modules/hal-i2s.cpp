#include "hal-i2s.h"
#include <iterator>
#include <algorithm>
#include "../hal.h"

#include <math.h>

extern I2S_HandleTypeDef hi2s1;

typedef enum {
    InterruptTxComplete = 0,
    InterruptTxHalfComplete = 1,
    InterruptMax,
} I2SDmaInterruptType;

typedef struct {
    HalGpio::Callback callback;
    void* context;
} I2SDmaInterrupt;

static volatile I2SDmaInterrupt interrupt[InterruptMax] = {0};

extern "C" void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef* hi2s) {
    if(hi2s == &hi2s1) {
        volatile I2SDmaInterrupt* _int = &interrupt[InterruptTxComplete];
        if(_int->callback != NULL) {
            _int->callback(_int->context);
        }
    }
}

extern "C" void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef* hi2s) {
    if(hi2s == &hi2s1) {
        volatile I2SDmaInterrupt* _int = &interrupt[InterruptTxHalfComplete];
        if(_int->callback != NULL) {
            _int->callback(_int->context);
        }
    }
}

HalI2S::HalI2S(I2S_HandleTypeDef* i2s) {
    this->i2s = i2s;
    const float pi = 3.14159265f;
    for(size_t i = 0; i < this->buffer_size; i++) {
        // saw
        buffer[i] = (i / 2);

        // sine
        // buffer[i] = sin(i * (pi / 128)) * (UINT_LEAST16_MAX / 2);
    }
}

HalI2S::~HalI2S() {
}

void HalI2S::start() {
    uint16_t* data = reinterpret_cast<uint16_t*>(this->buffer);
    HAL_I2S_Transmit_DMA(this->i2s, data, this->buffer_size);
}

void HalI2S::set_complete_callback(Callback cb, void* ctx) {
    interrupt[InterruptTxComplete].callback = cb;
    interrupt[InterruptTxComplete].context = ctx;
}

void HalI2S::set_half_complete_callback(Callback cb, void* ctx) {
    interrupt[InterruptTxHalfComplete].callback = cb;
    interrupt[InterruptTxHalfComplete].context = ctx;
}
