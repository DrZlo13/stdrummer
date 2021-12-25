#pragma once
#include <main.h>
#include "helper.h"

class HalEncoder {
private:
    DISALLOW_COPY_AND_ASSIGN(HalEncoder);
    TIM_HandleTypeDef* tim;

public:
    HalEncoder(TIM_HandleTypeDef* tim);
    ~HalEncoder();
    void start();
    uint32_t get();
};