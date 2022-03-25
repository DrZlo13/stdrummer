#include "app.h"
#include <main.h>
#include <string>
#include "hal/hal.h"
#include <log.h>
#include <core.h>
#include "gfx/gfx-export.h"
#include "player/wav-file.h"
#include <fmt/core.h>
#include <FreeRTOS.h>
#include <player/wav-cache.h>
#include <player/channel.h>
#include <os/task.h>

int uxTopUsedPriority = configMAX_PRIORITIES - 1;

void hal_init(void) {
    HalTime::init();

    gpio_led.config(HalGpio::Mode::OutputPushPull);
    gpio_button.config(HalGpio::Mode::Input, HalGpio::Pull::Up);
    uart_debug.config(115200);

    gpio_led.write(true);
}

const char* filename_long_file = "stereo_s16.wav";
const char* filename_kick = "kick.wav";
const char* filename_snare = "snare.wav";
const char* filename_hihat_close = "hihat_close.wav";
const char* filename_hihat_open = "hihat_open.wav";
const char* filename_1khz = "1khz.wav";

void wav_cacher(void* arg);
Task<1024> wav_cache_task(taskdef(wav_cacher), TaskPriority::High);

constexpr size_t channel_count = 6;
PlayerChannel channel[channel_count];

void wav_cacher(void* arg) {
    for(ever) {
        wav_cache_task.notify_wait();

        for(size_t i = 0; i < channel_count; i++) {
            // gpio_led.write(1);
            if(channel[i].fetch()) {
                // Log::info("fetched");
            }
            // gpio_led.write(0);
        }
    }
}

void i2s_worker(void* arg);
Task<8096> i2s_worker_task(taskdef(i2s_worker), TaskPriority::High);
constexpr uint32_t bit_half_complete = 1 << 0;
constexpr uint32_t bit_full_complete = 1 << 1;

void i2s_complete_callback(void* context) {
    i2s_worker_task.notify_from_isr(bit_full_complete);
}

void i2s_half_complete_callback(void* context) {
    i2s_worker_task.notify_from_isr(bit_half_complete);
}

ChannelOut out;

void prepare_block() {
    memset(out.sample, 0, sizeof(ChannelOut));
    for(size_t i = 0; i < channel_count; i++) {
        channel[i].get(out);
    }
    wav_cache_task.notify();
}

void i2s_worker(void* arg) {
    Log::info("Hello from i2s_worker");

    // channel[0].open(filename_kick);
    // channel[1].open(filename_hihat_open);
    // channel[2].open(filename_snare);
    // channel[3].open(filename_hihat_close);
    channel[4].open(filename_long_file);
    // channel[5].open(filename_1khz);

    i2s_dac.set_complete_callback(i2s_complete_callback, NULL);
    i2s_dac.set_half_complete_callback(i2s_half_complete_callback, NULL);

    prepare_block();
    i2s_dac.start();
    for(ever) {
        int16_t* buffer_p = NULL;
        uint32_t notify_bits = i2s_worker_task.notify_wait();

        if(notify_bits & bit_half_complete) {
            buffer_p = i2s_dac.get_buffer_first_half();
        } else if(notify_bits & bit_full_complete) {
            buffer_p = i2s_dac.get_buffer_second_half();
        } else {
            Core::crash("Unknown notify_bits");
        }

        SampleStereoF sample;

        gpio_led.write(1);
        for(size_t i = 0; i < out.size_in_samples; i++) {
            sample = out.sample[i];
            sample.left =
                std::clamp(sample.left / channel_count, (float)INT16_MIN, (float)INT16_MAX);
            sample.right =
                std::clamp(sample.right / channel_count, (float)INT16_MIN, (float)INT16_MAX);

            buffer_p[i * 2 + 0] = sample.left;
            buffer_p[i * 2 + 1] = sample.right;
        }
        prepare_block();
        gpio_led.write(0);
    }
}

void app_main(void* arg) {
    hal_init();
    Log::reset();
    Log::info("System start at " GIT_BRANCH "/" GIT_COMMIT);

    storage.start();
    // gfx.start();

    wav_cache_task.start();
    i2s_worker_task.start();

    Core::log_heap();

    // gfx.set_font(&font_basic_7x10);
    // gfx.set_cursor(0, 0);
    // gfx.draw_string(GIT_BRANCH "/" GIT_COMMIT, Color::White, Color::Black);
    // gfx.flush();

    osThreadExit();
}