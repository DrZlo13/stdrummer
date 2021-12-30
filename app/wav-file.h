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

    void read(int16_t* data, uint16_t data_bytes);
    void rewind();
};