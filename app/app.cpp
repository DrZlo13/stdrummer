#include "app.h"
#include <main.h>
#include <string>
#include "hal/hal.h"
#include <log.h>
#include <core.h>
#include <etl/array.h>
#include "gfx/gfx-export.h"
#include "wav-file.h"
#include <etl/queue_spsc_atomic.h>
#include <fmt/core.h>

typedef enum { EventHalfCplt, EventFullCplt } Event;

etl::queue_spsc_atomic<Event, 128> event_queue;

void hal_init(void) {
    HalTime::init();

    gpio_led.config(HalGpio::Mode::OutputPushPull);
    gpio_button.config(HalGpio::Mode::Input, HalGpio::Pull::Up);
    uart_debug.config(115200);

    gpio_led.write(false);
}

WavFile wav;

void i2s_complete_callback(void* context) {
    event_queue.push(EventFullCplt);
}

void i2s_half_complete_callback(void* context) {
    event_queue.push(EventHalfCplt);
}

void app_main(void) {
    hal_init();

    Log::reset();
    Log::info("System start at " GIT_BRANCH "/" GIT_COMMIT);

    // gfx.start();
    // int16_t angle = 0;
    // const float pi = 3.1415f;

    // while(1) {
    //     angle = angle + 2;
    //     Color color_front = Color::White;
    //     Color color_back = Color::Black;

    //     if(!encoder_button.read()) {
    //         color_front = Color::Black;
    //         color_back = Color::White;
    //     }

    //     gfx.fill(color_back);
    //     gfx.draw_line(
    //         64 + sin(pi / 180.0f * (float)angle) * 32.0f,
    //         32 + cos(pi / 180.0f * (float)angle) * 32.0f,
    //         64 - sin(pi / 180.0f * (float)angle) * 32.0f,
    //         32 - cos(pi / 180.0f * (float)angle) * 32.0f,
    //         color_front);

    //     gfx.set_font(&font_basic_6x8);
    //     gfx.set_cursor(0, 0);
    //     gfx.draw_string(fmt::format("Angle: {}", angle), color_front, color_back);

    //     gfx.flush();
    //     while(!gfx.flush_completed()) {
    //     }
    // }

    // encoder.start();
    // storage.start();
    // wav.open("stereo_s16.wav");

    // i2s_dac.set_complete_callback(i2s_complete_callback, NULL);
    // i2s_dac.set_half_complete_callback(i2s_half_complete_callback, NULL);

    // i2s_dac.start();

    // bool stop = true;

    // while(true) {
    //     Event event;

    //     if(encoder_button.read() != stop) {
    //         stop = !stop;
    //         if(!stop) {
    //             wav.rewind();
    //         }
    //     }

    //     if(event_queue.pop(event)) {
    //         if(stop) {
    //             memset(i2s_dac.get_buffer_first_half(), 0, i2s_dac.get_buffer_half_size());
    //             memset(i2s_dac.get_buffer_second_half(), 0, i2s_dac.get_buffer_half_size());
    //         } else {
    //             gpio_led.write(true);
    //             switch(event) {
    //             case EventFullCplt:
    //                 wav.read(i2s_dac.get_buffer_second_half(), i2s_dac.get_buffer_half_size());
    //                 break;
    //             case EventHalfCplt:
    //                 wav.read(i2s_dac.get_buffer_first_half(), i2s_dac.get_buffer_half_size());
    //                 break;
    //             }
    //             gpio_led.write(false);
    //         }
    //     }
    // }
}