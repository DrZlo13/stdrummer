#include "hal-sd.h"
#include "../hal.h"
#include <main.h>
#include <log.h>
#include <cmath>
#include <bsp_driver_sd.h>
#include <fatfs.h>
#include <vector>
#include <algorithm>
#include <set>
#include <core.h>
#include <filesystem.h>

HalSD::HalSD() {
}

void HalSD::start(void) {
    FRESULT err = f_mount(&SDFatFS, "/", 1);

    if(err == FR_OK) {
        Log::info(std::string("SD Mount: ") + std::string(fs_error_text(err)));
    } else {
        Log::error(std::string("SD Mount: ") + std::string(fs_error_text(err)));
    }
}
