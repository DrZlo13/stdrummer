#include "hal-time.h"
#include <main.h>

namespace HalTime {
uint32_t tick() {
    return HAL_GetTick();
}

uint32_t ticks_in_sec() {
    return 1000 / HAL_GetTickFreq();
}

uint32_t msec() {
    return HAL_GetTick();
}
}