#include <hal/hal.h>
#include "log.h"

namespace Log {
void log_send(std::string_view message) {
    uart_debug.transmit(message);
    uart_debug.transmit("\r\n");
}

void error(std::string_view message) {
    uart_debug.transmit(std::to_string(HalTime::tick()));
    uart_debug.transmit(LOG_COLOR_E " [UwU] " LOG_COLOR_RESET);
    log_send(message);
}

void warn(std::string_view message) {
    uart_debug.transmit(std::to_string(HalTime::tick()));
    uart_debug.transmit(LOG_COLOR_W " [OwO] " LOG_COLOR_RESET);
    log_send(message);
}

void info(std::string_view message) {
    uart_debug.transmit(std::to_string(HalTime::tick()));
    uart_debug.transmit(LOG_COLOR_I " [^w^] " LOG_COLOR_RESET);
    log_send(message);
}

void log(std::string_view message) {
    uart_debug.transmit(std::to_string(HalTime::tick()));
    uart_debug.transmit(LOG_COLOR_I " [-_-] " LOG_COLOR_RESET);
    log_send(message);
}

void plain(std::string_view message) {
    uart_debug.transmit(message);
}

void newline() {
    uart_debug.transmit("\r\n");
}

void reset() {
    uart_debug.transmit("\033[H\033[J");
}

}