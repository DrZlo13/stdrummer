#pragma once
#include "wav-cache.h"
#include "hal/hal.h"

struct ChannelOut {
    static constexpr size_t size_in_samples = HalI2S::get_buffer_half_size() / sizeof(float);
    SampleStereoF sample[size_in_samples];
};

static_assert(
    HalI2S::get_buffer_half_size() == (ChannelOut::size_in_samples * sizeof(float)),
    "ChannelOut: non-integer size");

class PlayerChannel {
private:
    WavCacheStereo16<6> cache;
    bool _loaded = false;
    bool _playing = true;
    bool _looped = true;
    const char* _filename;

    void process(
        SampleStereo16* sample_p,
        size_t sample_start,
        size_t samples_count,
        ChannelOut& out) {
        for(size_t i = 0; i < samples_count; i++) {
            float sample_l = sample_p[i].left;
            float sample_r = sample_p[i].right;

            out.sample[sample_start + i].left += sample_l;
            out.sample[sample_start + i].right += sample_r;
        }
    }

public:
    PlayerChannel() {
    }

    ~PlayerChannel() {
    }

    bool open(const char* path) {
        bool result = cache.open(path);

        if(result) {
            _filename = path;
            cache.init();
            _loaded = true;
        }

        return result;
    }

    void get(ChannelOut& out) {
        if(!_loaded) return;
        if(!_playing) return;

        SampleStereo16* sample_pointer = NULL;

        size_t samples_need_to_read = 0;

        uint8_t zero_read_count = 0;

        while(samples_need_to_read < out.size_in_samples) {
            size_t start_sample = samples_need_to_read;

            size_t samples_was_read =
                cache.read_start(&sample_pointer, out.size_in_samples - start_sample);

            if(samples_was_read == 0) {
                if(zero_read_count > 1) {
                    Log::info("{} underrun", _filename);
                    break;
                }
                zero_read_count++;
            }

            //     Log::info("{} {}", samples_was_read, samples_need_to_read);
            //     cache.log_info();
            //     Core::crash();
            // }

            process(sample_pointer, start_sample, samples_was_read, out);
            cache.read_done(samples_was_read);

            if(_looped && cache.end()) {
                cache.rewind();
            }

            samples_need_to_read += samples_was_read;
            // Log::info("{} {} {}", start_sample, samples_was_read, samples_need_to_read);
        }
    }

    bool fetch() {
        return cache.fetch();
    }

    bool playing() {
        return _playing;
    }

    bool looped() {
        return _looped;
    }

    void play_start() {
        cache.rewind();
        _playing = true;
    }

    void play_stop() {
        _playing = false;
    }
};
