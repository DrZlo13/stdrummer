#include "hal-time.h"
#include <main.h>
#include <cmsis_os.h>

namespace HalTime {
static uint32_t cycles_per_microsecond;
static uint32_t dwt_mark;

static void init_cycle_counter() {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0U;
    cycles_per_microsecond = SystemCoreClock / 1000000.0f;
}

void init() {
    init_cycle_counter();
}

uint32_t tick() {
    return xTaskGetTickCount();
}

uint32_t ticks_in_sec() {
    return configTICK_RATE_HZ;
}

uint32_t msec() {
    return xTaskGetTickCount() * portTICK_RATE_MS;
}

uint32_t cycle_count() {
    return DWT->CYCCNT;
}

uint32_t cycle_freq() {
    return SystemCoreClock;
}

void delay(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void delay_us(uint32_t us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t time_ticks = us * cycles_per_microsecond;
    while((DWT->CYCCNT - start) < time_ticks) {
    };
}

void mark() {
    DWT->CYCCNT = 0;
    dwt_mark = DWT->CYCCNT;
}

uint32_t us_from_mark() {
    return (DWT->CYCCNT - dwt_mark) / cycles_per_microsecond;
}
}