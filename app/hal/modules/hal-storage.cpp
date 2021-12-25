#include "hal-storage.h"
#include "../hal.h"
#include <main.h>
#include <cmath>
#include <bsp_driver_sd.h>

HalStorage::HalStorage() {
}

HalStorage::~HalStorage() {
}

void HalStorage::bench_write() {
    FRESULT err;
    FIL file;

    //err = f_open(&file, "/read.test", FA_READ | FA_CREATE_ALWAYS);
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
                str = "f_open: " + std::string(err_text(err)) + "\r\n";
                uart_debug.transmit(str);
                break;
            }

            time = DWT->CYCCNT;
            for(size_t i = 0; i < bench_count; i++) {
                UINT written = 0;
                err = f_write(&file, chunk, chunk_size, &written);
                if(err != FR_OK || written != chunk_size) {
                    str = "f_write: " + std::string(err_text(err)) + "\r\n";
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
                str = "f_open: " + std::string(err_text(err)) + "\r\n";
                uart_debug.transmit(str);
                break;
            }

            time = DWT->CYCCNT;
            for(size_t i = 0; i < bench_count; i++) {
                UINT was_read = 0;
                err = f_read(&file, chunk, chunk_size, &was_read);
                if(err != FR_OK || was_read != chunk_size) {
                    str = "f_read: " + std::string(err_text(err)) + "\r\n";
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
    /*while(true) {
        if(BSP_SD_IsDetected()) {
            uart_debug.transmit("Detected\r\n");
        } else {
        }
    }*/

    std::string str;
    FRESULT err;

    err = f_mount(&SDFatFS, "/", 1);
    str = "f_mount: " + std::string(err_text(err)) + "\r\n";
    uart_debug.transmit(str);

    DIR dir;
    FILINFO fno;
    err = f_opendir(&dir, "/");
    str = "f_opendir: " + std::string(err_text(err)) + "\r\n";
    uart_debug.transmit(str);

    for(;;) {
        err = f_readdir(&dir, &fno);
        if(err != FR_OK || fno.fname[0] == 0) break;

        if(fno.fattrib & AM_DIR) {
            str = "[D] " + std::string(fno.fname) + "\r\n";
            uart_debug.transmit(str);
        } else {
            str = "[F] " + std::string(fno.fname) + ", " + std ::to_string(fno.fsize) + "b\r\n";
            uart_debug.transmit(str);
        }
    }
    f_closedir(&dir);

    bench_write();
    uart_debug.transmit("benched\r\n");
}

static const char* err_texts[] = {
    [FR_OK] = "OK",
    [FR_DISK_ERR] = "DISK_ERR",
    [FR_INT_ERR] = "INT_ERR",
    [FR_NOT_READY] = "NOT_READY",
    [FR_NO_FILE] = "NO_FILE",
    [FR_NO_PATH] = "NO_PATH",
    [FR_INVALID_NAME] = "INVALID_NAME",
    [FR_DENIED] = "DENIED",
    [FR_EXIST] = "EXIST",
    [FR_INVALID_OBJECT] = "INVALID_OBJECT",
    [FR_WRITE_PROTECTED] = "WRITE_PROTECTED",
    [FR_INVALID_DRIVE] = "INVALID_DRIVE",
    [FR_NOT_ENABLED] = "NOT_ENABLED",
    [FR_NO_FILESYSTEM] = "NO_FILESYSTEM",
    [FR_MKFS_ABORTED] = "MKFS_ABORTED",
    [FR_TIMEOUT] = "TIMEOUT",
    [FR_LOCKED] = "LOCKED",
    [FR_NOT_ENOUGH_CORE] = "NOT_ENOUGH_CORE",
    [FR_TOO_MANY_OPEN_FILES] = "TOO_MANY_OPEN_FILES",
    [FR_INVALID_PARAMETER] = "INVALID_PARAMETER",
};

const char* HalStorage::err_text(FRESULT err) {
    if(err >= COUNT_OF(err_texts)) {
        return "UNKNOWN";
    } else {
        return err_texts[err];
    }
}
