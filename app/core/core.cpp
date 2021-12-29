#include <hal/hal.h>
#include "log.h"

extern "C" void Error_Handler(void);

void core_crash(const char* message) {
    char tick[] = "-2147483648";
    itoa(HalTime::tick(), tick, 10);
    uart_debug.transmit(tick);
    uart_debug.transmit(" " LOG_COLOR_E);
    uart_debug.transmit("[Owo] Big bad CRASH happened");
    uart_debug.transmit(LOG_COLOR_RESET "\r\n");

    itoa(HalTime::tick(), tick, 10);
    uart_debug.transmit(tick);
    uart_debug.transmit(" " LOG_COLOR_E);
    uart_debug.transmit("[oWO] ");
    uart_debug.transmit(LOG_COLOR_RESET);
    if(message != NULL) {
        uart_debug.transmit(message);
        uart_debug.transmit("\r\n");
    } else {
        uart_debug.transmit("¯\\(ツ)/¯\r\n");
    }

    Error_Handler();
}