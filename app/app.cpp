#include "app.h"
#include <main.h>
#include <string>
#include "hal/hal.h"
#include <log.h>
#include <core.h>
#include <etl/array.h>
#include "gfx/gfx-export.h"

void hal_init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0U;

    gpio_led.config(HalGpio::Mode::OutputPushPull);
    gpio_button.config(HalGpio::Mode::Input, HalGpio::Pull::Up);
    uart_debug.config(115200);
}

void app_main(void) {
    hal_init();
    Log::reset();
    Log::info("System start");

    // i2s_dac.start();
    storage.start();
    gfx.start(false, false, false);
    encoder.start();

    int16_t angle = 0;
    const float pi = 3.1415f;
    std::string str;

    while(1) {
        angle -= encoder.get();
        // encoder.reset();

        Color color_front = Color::White;
        Color color_back = Color::Black;

        if(!encoder_button.read()) {
            color_front = Color::Black;
            color_back = Color::White;
        }

        gfx.fill(color_back);
        gfx.draw_line(
            64 + sin(pi / 180.0f * (float)angle) * 32.0f,
            32 + cos(pi / 180.0f * (float)angle) * 32.0f,
            64 - sin(pi / 180.0f * (float)angle) * 32.0f,
            32 - cos(pi / 180.0f * (float)angle) * 32.0f,
            color_front);

        gfx.set_font(&font_basic_6x8);
        gfx.set_cursor(0, 0);
        str = "Angle: " + std::to_string(angle);
        gfx.draw_string(str, color_front, color_back);

        gfx.flush();
        while(!gfx.flush_completed()) {
        }
    }
}