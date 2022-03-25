#pragma once
#include <string>
#include <fmt/format.h>
#include <fmt/compile.h>

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

void plain(std::string_view message);

void newline();

void reset();

template <typename... Args> void error(fmt::format_string<Args...> message, Args&&... args) {
    Log::error(fmt::format(message, std::forward<Args>(args)...));
}

template <typename... Args> void warn(fmt::format_string<Args...> message, Args&&... args) {
    Log::warn(fmt::format(message, std::forward<Args>(args)...));
}

template <typename... Args> void info(fmt::format_string<Args...> message, Args&&... args) {
    Log::info(fmt::format(message, std::forward<Args>(args)...));
}

template <typename... Args> void log(fmt::format_string<Args...> message, Args&&... args) {
    Log::log(fmt::format(message, std::forward<Args>(args)...));
}

template <typename... Args> void plain(fmt::format_string<Args...> message, Args&&... args) {
    Log::plain(fmt::format(message, std::forward<Args>(args)...));
}
}