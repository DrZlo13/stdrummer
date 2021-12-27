#pragma once
#include <stdint.h>
#include <helper.h>
#include <fatfs.h>

class HalStorage {
private:
    DISALLOW_COPY_AND_ASSIGN(HalStorage);

public:
    HalStorage();
    ~HalStorage();

    void start(void);
    const char* err_text(FRESULT err);
    void bench_write();
};