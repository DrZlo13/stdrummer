#pragma once
#include <stdint.h>
#include "sample.h"
#include "wav-file.h"
#include <log.h>
#include <core.h>
#include <math.h>
#include <string.h>
#include <atomic>
#include <core/log.h>
#include <fmt/core.h>

template <size_t blocks_count> class WavCacheStereo16 {
private:
    enum class CacheState : uint32_t { Loaded, QueuedToLoad, Unused };

    template <size_t _blocks_count> struct CacheBlockStereo16 {
        std::atomic<CacheState> state;
        size_t samples_count;
        SampleBlockStereo16<_blocks_count> block;
    };

    const char* cache_state_text(CacheState state) {
        switch(state) {
        case CacheState::Loaded:
            return "Loaded";
        case CacheState::QueuedToLoad:
            return "Queued";
        case CacheState::Unused:
            return "Unused";
        default:
            return "Unknown";
        }
    }

    constexpr static size_t block_max_index = 3;
    constexpr static size_t block_size_in_bytes = SampleBlockStereo16<blocks_count>::size_in_bytes;
    constexpr static size_t block_size_in_samples =
        SampleBlockStereo16<blocks_count>::size_in_samples;
    constexpr static size_t sample_size_in_bytes = block_size_in_bytes / block_size_in_samples;

    CacheBlockStereo16<blocks_count> cache[block_max_index];
    size_t current_block = 0;
    size_t current_sample = 0;
    size_t file_last_block = 0;
    bool need_to_rewind = false;

    WavFile file;

    bool load_block(size_t i) {
        uint32_t was_read = file.read(cache[i].block.sample_p, block_size_in_bytes);
        cache[i].samples_count = 0;
        if(was_read > 0) {
            cache[i].samples_count = was_read / sample_size_in_bytes;
            cache[i].state = CacheState::Loaded;
        }

        return was_read > 0;
    }

    CacheBlockStereo16<blocks_count>* get_current_block() {
        uint8_t block_index;
        if(current_block == 0) {
            // (current_block == 0) => cache[0]
            block_index = current_block;
        } else {
            // (current_block == 1) => cache[1]
            // (current_block == 2) => cache[2]
            // (current_block == 3) => cache[1]
            // (current_block == 4) => cache[2]
            // ...
            block_index = ((current_block + 1) % 2) + 1;
        }

        return &cache[block_index];
    }

public:
    WavCacheStereo16() {
    }

    ~WavCacheStereo16() {
    }

    size_t get_cache_block_size() {
        return block_size_in_bytes;
    }

    bool open(std::string_view path) {
        return file.open(path);
    }

    void init(void) {
        need_to_rewind = false;
        current_block = 0;
        current_sample = 0;
        file.rewind(0);

        file_last_block = (file.length_in_bytes() / block_size_in_bytes);
        if((file.length_in_bytes() % block_size_in_bytes) > 0) {
            file_last_block += 1;
        }

        for(size_t i = 0; i < block_max_index; i++) {
            cache[i].state = CacheState::Unused;
        }

        for(size_t i = 0; i < std::min(file_last_block, block_max_index); i++) {
            load_block(i);
        }
    }

    void rewind() {
        need_to_rewind = true;
        current_block = 0;
        current_sample = 0;
    }

    bool end() {
        return current_block >= file_last_block;
    }

    size_t read_start(SampleStereo16** sample_pointer, size_t samples_needed) {
        size_t remain_samples = 0;
        CacheBlockStereo16<blocks_count>* cache_p = get_current_block();

        if(cache_p->state == CacheState::Loaded) {
            *sample_pointer = &(cache_p->block.sample[current_sample]);
            remain_samples = std::min(samples_needed, cache_p->samples_count - current_sample);
        }

        // Log::info("{} {} {}", current_sample, current_block, remain_samples);

        return remain_samples;
    }

    void read_done(size_t samples_was_read) {
        CacheBlockStereo16<blocks_count>* cache_p = get_current_block();
        current_sample += samples_was_read;

        if(current_sample >= cache_p->samples_count) {
            if(current_block != 0) {
                cache_p->state = CacheState::QueuedToLoad;
            }

            current_sample = 0;
            current_block += 1;
        }
    }

    bool fetch() {
        if(need_to_rewind) {
            need_to_rewind = false;
            file.rewind(cache[0].block.size_in_bytes);
        }

        bool fetched = false;
        for(size_t i = 1; i < block_max_index; i++) {
            if(cache[i].state == CacheState::QueuedToLoad) {
                if(load_block(i)) {
                    fetched = true;
                }
            }
        }

        return fetched;
    }

    void log_info() {
        Log::info("file_last_block: {}", file_last_block);
        Log::info("block: {}, sample: {}", current_block, current_sample);

        for(size_t i = 0; i < block_max_index; i++) {
            Log::info("[{}]:{} {}", i, cache_state_text(cache[i].state), cache[i].samples_count);
        }
    }
};
