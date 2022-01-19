#include "wav-file.h"
#include <log.h>
#include <hal/hal.h>
#include <fmt/core.h>

typedef struct {
    uint8_t riff[4];
    uint32_t size;
    uint8_t wave[4];
} WavHeaderChunk;

typedef struct {
    uint8_t fmt[4];
    uint32_t size;
    uint16_t tag;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_per_sec;
    uint16_t block_align;
    uint16_t bits_per_sample;
} WavFormatChunk;

typedef struct {
    uint8_t data[4];
    uint32_t size;
} WavDataChunk;

WavFile::WavFile() {
}

WavFile::~WavFile() {
}

bool WavFile::open(std::string_view path) {
    bool result = false;
    do {
        if(!file.open(path, File::Access::Read, File::Mode::OpenExisting)) {
            Log::error("WAV: cannot open file, " + std::string(file.error_text()));
            break;
        }

        if(!file.fastseek_enable(16)) {
            Log::error("WAV: cannot set fast mode, " + std::string(file.error_text()));
            break;
        }

        WavHeaderChunk header;
        WavFormatChunk format;
        WavDataChunk data;

        file.read(reinterpret_cast<uint8_t*>(&header), sizeof(header));
        file.read(reinterpret_cast<uint8_t*>(&format), sizeof(format));
        file.read(reinterpret_cast<uint8_t*>(&data), sizeof(data));

        Log::info(path);

        if(memcmp(header.riff, "RIFF", 4) != 0 || memcmp(header.wave, "WAVE", 4) != 0) {
            Log::error("WAV: wrong header");
            break;
        }

        if(memcmp(format.fmt, "fmt ", 4) != 0) {
            Log::error("WAV: wrong format");
            break;
        }

        if(format.tag != 0x01 || memcmp(data.data, "data", 4) != 0) {
            Log::error(
                fmt::format("WAV: non-PCM format {:#04x}, next '{}'", format.tag, data.data));
            break;
        }

        if(format.bits_per_sample != 16) {
            Log::error("WAV: not a 16-bit PCM");
            break;
        }

        Log::info(fmt::format(
            "tag: {:#04x}, ch: {}, smplrate: {}, bps: {}, bits: {}",
            format.tag,
            format.channels,
            format.sample_rate,
            format.byte_per_sec,
            format.bits_per_sample));

        wav_channels = format.channels;
        wav_sample_rate = format.sample_rate;
        wav_data_start = file.tell();
        wav_data_end = wav_data_start + data.size;

        Log::info(fmt::format("data: {} - {}", wav_data_start, wav_data_end));

        result = true;
    } while(false);

    return result;
}

bool WavFile::close() {
    return file.close();
}

void WavFile::read(int16_t* data, uint16_t size) {
    uint8_t* data_p = reinterpret_cast<uint8_t*>(data);
    int32_t to_read = size;

    while(to_read > 0) {
        uint32_t was_read = file.read(data_p + (size - to_read), to_read);
        to_read -= was_read;

        if(file.eof()) {
            rewind();
        }
    }
}

void WavFile::rewind() {
    file.seek(wav_data_start, true);
}
