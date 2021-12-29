#include <hal/hal.h>
#include "log.h"

namespace Log {
void log_send(std::string_view message) {
    uart_debug.transmit(message);
    uart_debug.transmit("\r\n");
}

void error(std::string_view message) {
    std::string prefix = "" + std::to_string(HalTime::tick()) + " ";
    prefix += LOG_COLOR_E "[UwU]" LOG_COLOR_RESET " ";
    uart_debug.transmit(prefix);
    log_send(message);
}

void warn(std::string_view message) {
    std::string prefix = "" + std::to_string(HalTime::tick()) + " ";
    prefix += LOG_COLOR_W "[OwO]" LOG_COLOR_RESET " ";
    uart_debug.transmit(prefix);
    log_send(message);
}

void info(std::string_view message) {
    std::string prefix = "" + std::to_string(HalTime::tick()) + " ";
    prefix += LOG_COLOR_I "[^w^]" LOG_COLOR_RESET " ";
    uart_debug.transmit(prefix);
    log_send(message);
}

void log(std::string_view message) {
    std::string prefix = "" + std::to_string(HalTime::tick()) + " ";
    prefix += "[-_-] ";
    uart_debug.transmit(prefix);
    log_send(message);
}

void reset() {
    uart_debug.transmit("\033[H\033[J");
}

}