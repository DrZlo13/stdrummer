#pragma once
#include <stdint.h>
#include <helper.h>
#include <fatfs.h>
#include <set>
#include <string>

class HalSD {
private:
    DISALLOW_COPY_AND_ASSIGN(HalSD);

public:
    HalSD();
    void start(void);
};