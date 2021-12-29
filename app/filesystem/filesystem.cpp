#include <math.h>
#include <fatfs.h>
#include <log.h>
#include "filesystem.h"

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

const char* fs_error_text(uint32_t error) {
    if(error >= COUNT_OF(error_texts)) {
        return "UNKNOWN";
    } else {
        return error_texts[error];
    }
}

void fs_bench_card(void) {
    std::string str;
    File file;

    for(size_t p = 3; p < 16; p++) {
        uint16_t chunk_size = std::pow(2, p);
        uint8_t* chunk = static_cast<uint8_t*>(malloc(chunk_size));
        std::string filename = "/read." + std ::to_string(chunk_size) + ".test";
        const uint32_t bench_count = std::pow(2, 15) / chunk_size * 4;
        uint32_t time = 0;

        do {
            if(!file.open(filename, File::Access::Write, File::Mode::CreateAlways)) {
                Log::error("open: " + std::string(file.error_text()));
                break;
            }

            time = DWT->CYCCNT;
            for(size_t i = 0; i < bench_count; i++) {
                if(file.write(chunk, chunk_size) != chunk_size) {
                    Log::error("write: " + std::string(file.error_text()));
                    break;
                }
            }
            file.close();

            float wtime = DWT->CYCCNT - time;
            wtime = wtime / SystemCoreClock;
            float wbps = chunk_size * bench_count / wtime / 1024.0f;

            if(!file.open(filename, File::Access::Read, File::Mode::OpenExisting)) {
                Log::error("open: " + std::string(file.error_text()));
                break;
            }

            time = DWT->CYCCNT;
            for(size_t i = 0; i < bench_count; i++) {
                if(file.read(chunk, chunk_size) != chunk_size) {
                    Log::error("read: " + std::string(file.error_text()));
                    break;
                }
            }
            file.close();

            float rtime = DWT->CYCCNT - time;
            rtime = rtime / SystemCoreClock;
            float rbps = chunk_size * bench_count / rtime / 1024.0f;

            Log::info(
                "[" + std::to_string(chunk_size) + "] [W]" + std::to_string(wtime) + "s " +
                std::to_string((int)wbps) + "K [R]" + std::to_string(rtime) + "s " +
                std::to_string((int)rbps) + "K");
        } while(false);

        free(chunk);
    }
}