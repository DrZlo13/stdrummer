#pragma once
#include <stdint.h>

constexpr size_t __block_size_in_bytes = 512;

typedef struct SampleStereoF SampleStereoF;
typedef struct SampleStereo16 SampleStereo16;
typedef struct SampleStereo32 SampleStereo32;

struct SampleStereoF {
    float left;
    float right;
};

struct SampleStereo32 {
    int32_t left;
    int32_t right;
};

// __attribute__((packed)) ?
struct SampleStereo16 {
    int16_t left;
    int16_t right;

    SampleStereo16& operator+(const SampleStereo16& param) {
        this->left += param.left;
        this->right += param.right;
        return *this;
    };

    SampleStereo16& operator+(int16_t param) {
        this->left += param;
        this->right += param;
        return *this;
    };

    SampleStereo16& operator+=(int16_t param) {
        this->left += param;
        this->right += param;
        return *this;
    };

    SampleStereo16& operator*(int16_t param) {
        this->left *= param;
        this->right *= param;
        return *this;
    };

    SampleStereo16& operator*=(int16_t param) {
        this->left *= param;
        this->right *= param;
        return *this;
    };

    SampleStereo16& operator/(int16_t param) {
        this->left /= param;
        this->right /= param;
        return *this;
    };

    SampleStereo16& operator=(int16_t param) {
        this->left = param;
        this->right = param;
        return *this;
    };

    SampleStereo16& operator=(const SampleStereoF& param) {
        this->left = param.left;
        this->right = param.right;
        return *this;
    };

    SampleStereo16& operator=(const SampleStereo32& param) {
        this->left = param.left;
        this->right = param.right;
        return *this;
    };
};

template <size_t blocks_count> struct SampleBlockStereo16 {
    static constexpr size_t size_in_blocks = blocks_count;
    static constexpr size_t size_in_bytes = __block_size_in_bytes * size_in_blocks;
    static constexpr size_t size_in_samples = size_in_bytes / sizeof(SampleStereo16);

    union {
        SampleStereo16 sample[size_in_samples];
        uint8_t sample_p[size_in_bytes];
    };
};

static_assert(sizeof(SampleStereo16) == 4, "SampleStereo16 must be 4-byte long");

static_assert(
    SampleBlockStereo16<1>::size_in_bytes ==
        (SampleBlockStereo16<1>::size_in_samples * sizeof(SampleStereo16)),
    "SampleBlockStereo16: non-integer size");