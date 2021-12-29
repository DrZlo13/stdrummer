#include "hal-storage.h"
#include "../hal.h"
#include <main.h>
#include <log.h>
#include <cmath>
#include <bsp_driver_sd.h>
#include <fatfs.h>

static const char* error_texts[] = {
    [FR_OK] = "OK",
    [FR_DISK_ERR] = "DISK ERR",
    [FR_INT_ERR] = "INT ERR",
    [FR_NOT_READY] = "NOT READY",
    [FR_NO_FILE] = "NO FILE",
    [FR_NO_PATH] = "NO PATH",
    [FR_INVALID_NAME] = "INVALID NAME",
    [FR_DENIED] = "DENIED",
    [FR_EXIST] = "EXIST",
    [FR_INVALID_OBJECT] = "INVALID OBJECT",
    [FR_WRITE_PROTECTED] = "WRITE PROTECTED",
    [FR_INVALID_DRIVE] = "INVALID DRIVE",
    [FR_NOT_ENABLED] = "NOT ENABLED",
    [FR_NO_FILESYSTEM] = "NO FILESYSTEM",
    [FR_MKFS_ABORTED] = "MKFS ABORTED",
    [FR_TIMEOUT] = "TIMEOUT",
    [FR_LOCKED] = "LOCKED",
    [FR_NOT_ENOUGH_CORE] = "NOT ENOUGH CORE",
    [FR_TOO_MANY_OPEN_FILES] = "TOO MANY OPEN FILES",
    [FR_INVALID_PARAMETER] = "INVALID PARAMETER",
};

HalStorage::HalStorage() {
}

HalStorage::~HalStorage() {
}

void HalStorage::bench_card() {
    FRESULT err;
    FIL file;

    std::string str;
    for(size_t p = 3; p < 16; p++) {
        uint16_t chunk_size = std::pow(2, p);
        void* chunk = malloc(chunk_size);
        std::string filename = "/read." + std ::to_string(chunk_size) + ".test";
        const uint32_t bench_count = std::pow(2, 15) / chunk_size * 4;
        uint32_t time = 0;

        do {
            err = f_open(&file, filename.data(), FA_WRITE | FA_CREATE_ALWAYS);
            if(err != FR_OK) {
                str = "f_open: " + std::string(error_text(err)) + "\r\n";
                uart_debug.transmit(str);
                break;
            }

            time = DWT->CYCCNT;
            for(size_t i = 0; i < bench_count; i++) {
                UINT written = 0;
                err = f_write(&file, chunk, chunk_size, &written);
                if(err != FR_OK || written != chunk_size) {
                    str = "f_write: " + std::string(error_text(err)) + "\r\n";
                    uart_debug.transmit(str);
                    break;
                }
            }
            f_close(&file);

            float wtime = DWT->CYCCNT - time;
            wtime = wtime / SystemCoreClock;
            float wbps = chunk_size * bench_count / wtime / 1024.0f;

            err = f_open(&file, filename.data(), FA_READ | FA_OPEN_EXISTING);
            if(err != FR_OK) {
                str = "f_open: " + std::string(error_text(err)) + "\r\n";
                uart_debug.transmit(str);
                break;
            }

            time = DWT->CYCCNT;
            for(size_t i = 0; i < bench_count; i++) {
                UINT was_read = 0;
                err = f_read(&file, chunk, chunk_size, &was_read);
                if(err != FR_OK || was_read != chunk_size) {
                    str = "f_read: " + std::string(error_text(err)) + "\r\n";
                    uart_debug.transmit(str);
                    break;
                }
            }
            f_close(&file);

            float rtime = DWT->CYCCNT - time;
            rtime = rtime / SystemCoreClock;
            float rbps = chunk_size * bench_count / rtime / 1024.0f;

            str = "[" + std::to_string(chunk_size) + "] [W]" + std::to_string(wtime) + "s " +
                  std::to_string((int)wbps) + "K [R]" + std::to_string(rtime) + "s " +
                  std::to_string((int)rbps) + "K" + "\r\n";
            uart_debug.transmit(str);
        } while(false);

        f_close(&file);
        free(chunk);
    }
}

void HalStorage::start(void) {
    std::string str;
    FRESULT err;

    err = f_mount(&SDFatFS, "/", 1);
    str = "f_mount: " + std::string(error_text(err));
    Log::info(str);

    /*DIR dir;
    FILINFO fno;
    err = f_opendir(&dir, "/");
    str = "f_opendir: " + std::string(error_text(err));
    Log::info(str);

    for(;;) {
        err = f_readdir(&dir, &fno);
        if(err != FR_OK || fno.fname[0] == 0) break;

        if(fno.fattrib & AM_DIR) {
            str = "[D] " + std::string(fno.fname);
            Log::info(str);
        } else {
            str = "[F] " + std::string(fno.fname) + ", " + std ::to_string(fno.fsize) + "b";
            Log::info(str);
        }
    }
    f_closedir(&dir);*/
}

const char* HalStorage::error_text(uint32_t error) {
    if(error >= COUNT_OF(error_texts)) {
        return "UNKNOWN";
    } else {
        return error_texts[error];
    }
}
