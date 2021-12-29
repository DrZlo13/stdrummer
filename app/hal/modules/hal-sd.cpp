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
    Log::info(std::string("f_mount: ") + std::string(fs_error_text(err)));

    Dir dir;
    FileInfo info;

    dir.open("/");
    Log::info("open: " + std::string(dir.error_text()));

    while(true) {
        if(!dir.read(info)) break;

        if(info.is_dir()) {
            Log::info("[D] " + info.name());
        } else {
            Log::info("[F] " + info.name() + ", " + std::to_string(info.size()) + "b");
        }
    }
    dir.close();

    fs_bench_card();
}
