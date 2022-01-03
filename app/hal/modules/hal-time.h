#pragma once
#include <stdint.h>

namespace HalTime {
void init();
uint32_t tick();
uint32_t ticks_in_sec();
uint32_t msec();
uint32_t cycle_count();
uint32_t cycle_freq();
};
