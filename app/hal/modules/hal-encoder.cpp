#include "hal-encoder.h"

HalEncoder::HalEncoder(TIM_HandleTypeDef* tim) {
    this->tim = tim;
}

HalEncoder::~HalEncoder() {
}

void HalEncoder::start() {
    HAL_TIM_Encoder_Start(tim, TIM_CHANNEL_ALL);
}

uint32_t HalEncoder::get() {
    return this->tim->Instance->CNT;
}

void HalEncoder::reset() {
    this->tim->Instance->CNT = 0;
}
