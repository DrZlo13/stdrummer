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

typedef enum { EventHalfCplt, EventFullCplt } Event;

etl::queue_spsc_atomic<Event, 128> event_queue;

void hal_init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0U;

    gpio_led.config(HalGpio::Mode::OutputPushPull);
    gpio_button.config(HalGpio::Mode::Input, HalGpio::Pull::Up);
    uart_debug.config(115200);
}

WavFile wav;

void i2s_complete_callback(void* context) {
    // gpio_led.write(true);
    event_queue.push(EventFullCplt);
    // gpio_led.write(false);
}

void i2s_half_complete_callback(void* context) {
    // gpio_led.write(true);
    event_queue.push(EventHalfCplt);
    // gpio_led.write(false);
}

void app_main(void) {
    hal_init();
    gpio_led.write(false);

    Log::reset();
    Log::info("System start at " GIT_BRANCH "/" GIT_COMMIT);

    encoder.start();
    storage.start();
    wav.open("stereo_s16.wav");

    i2s_dac.set_complete_callback(i2s_complete_callback, NULL);
    i2s_dac.set_half_complete_callback(i2s_half_complete_callback, NULL);

    i2s_dac.start();

    bool stop = true;

    while(true) {
        Event event;

        if(encoder_button.read() != stop) {
            stop = !stop;
            if(!stop) {
                wav.rewind();
            }
        }

        if(event_queue.pop(event)) {
            if(stop) {
                memset(i2s_dac.get_buffer_first_half(), 0, i2s_dac.get_buffer_half_size());
                memset(i2s_dac.get_buffer_second_half(), 0, i2s_dac.get_buffer_half_size());
            } else {
                gpio_led.write(true);
                switch(event) {
                case EventFullCplt:
                    wav.read(i2s_dac.get_buffer_second_half(), i2s_dac.get_buffer_half_size());
                    break;
                case EventHalfCplt:
                    wav.read(i2s_dac.get_buffer_first_half(), i2s_dac.get_buffer_half_size());
                    break;
                }
                gpio_led.write(false);
            }
        }
    }
}