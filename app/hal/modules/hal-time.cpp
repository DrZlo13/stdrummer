#include "hal-time.h"
#include <main.h>

namespace HalTime {
void init() {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0U;
}

uint32_t tick() {
    return HAL_GetTick();
}

uint32_t ticks_in_sec() {
    return 1000 / HAL_GetTickFreq();
}

uint32_t msec() {
    return HAL_GetTick();
}

uint32_t cycle_count() {
    return DWT->CYCCNT;
}

uint32_t cycle_freq() {
    return SystemCoreClock;
}
}