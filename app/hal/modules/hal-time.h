#pragma once
#include <stdint.h>

namespace HalTime {
uint32_t tick();
uint32_t ticks_in_sec();
uint32_t msec();
};