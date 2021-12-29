#pragma once
#include <string>

#define LOG_COLOR(clr) "\033[0;" clr "m"
#define LOG_COLOR_RESET "\033[0m"

#define LOG_COLOR_BLACK "30"
#define LOG_COLOR_RED "31"
#define LOG_COLOR_GREEN "32"
#define LOG_COLOR_BROWN "33"
#define LOG_COLOR_BLUE "34"
#define LOG_COLOR_PURPLE "35"

#define LOG_COLOR_E LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I LOG_COLOR(LOG_COLOR_GREEN)

namespace Log {

void error(std::string_view message);

void warn(std::string_view message);

void info(std::string_view message);

void log(std::string_view message);

void reset();

}