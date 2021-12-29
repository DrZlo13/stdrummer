#pragma once
#include <stdint.h>
#include <helper.h>

class File {
private:
    DISALLOW_COPY_AND_ASSIGN(File);
    uint32_t handle;
    uint32_t error;

public:
    File();
    ~File();
};

class HalStorage {
private:
    DISALLOW_COPY_AND_ASSIGN(HalStorage);

public:
    HalStorage();
    ~HalStorage();
    void bench_card();

    void start(void);
    const char* error_text(uint32_t error);
};