#include "hal-encoder.h"

constexpr uint32_t encoder_middle = UINT16_MAX / 2;

HalEncoder::HalEncoder(TIM_HandleTypeDef* tim) {
    this->tim = tim;
}

HalEncoder::~HalEncoder() {
}

void HalEncoder::start() {
    HAL_TIM_Encoder_Start(tim, TIM_CHANNEL_ALL);
    reset();
}

int32_t HalEncoder::get() {
    int32_t value = this->tim->Instance->CNT;
    reset();
    return value - encoder_middle;
}

void HalEncoder::reset() {
    this->tim->Instance->CNT = encoder_middle;
}
