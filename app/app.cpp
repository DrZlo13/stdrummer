#include "app.h"
#include <main.h>
#include <string>
#include "hal/hal.h"
#include "core/core.h"
#include <etl/include/etl/queue_spsc_atomic.h>

void app_main(void) {
    hal_init();
    uart_debug.transmit("UART1/DEBUG\r\n");

    i2s_dac.start();
}