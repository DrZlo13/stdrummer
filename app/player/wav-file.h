#pragma once
#include <filesystem.h>

class WavFile {
private:
    File file;

    uint8_t wav_channels;
    uint8_t wav_sample_rate;

    uint32_t wav_data_start;
    uint32_t wav_data_end;

public:
    WavFile();
    ~WavFile();

    bool open(std::string_view path);
    bool close();

    uint32_t read(uint8_t* data, uint16_t data_bytes);

    void read_looping(uint8_t* data, uint16_t data_bytes);

    void rewind(uint32_t offset = 0);
    bool eof();
    bool end();
    uint32_t length_in_bytes();
    uint32_t current_pos();
};