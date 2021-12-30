#include "hal-sd.h"
#include "../hal.h"
#include <main.h>
#include <log.h>
#include <bsp_driver_sd.h>
#include <fatfs.h>
#include <core.h>
#include <filesystem.h>
#include <fmt/core.h>

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

// /**
//   * @brief return the SD state
//   * @param hsd: Pointer to sd handle
//   * @retval HAL state
//   */
// HAL_SD_StateTypeDef HAL_SD_GetState(SD_HandleTypeDef *hsd)
// {
//   return hsd->State;
// }

// /**
// * @brief  Return the SD error code
// * @param  hsd : Pointer to a SD_HandleTypeDef structure that contains
//   *              the configuration information.
// * @retval SD Error Code
// */
// uint32_t HAL_SD_GetError(SD_HandleTypeDef *hsd)
// {
//   return hsd->ErrorCode;
// }

// extern "C" void BSP_SD_WriteCpltCallback(void) {
//     Log::info("SD Write Complete");
// }

// extern "C" void BSP_SD_ReadCpltCallback(void) {
//     Log::info("SDRxCplt");
// }

extern "C" void BSP_SD_AbortCallback(void) {
    Log::error("SD Abort");
}

extern "C" void HAL_SD_ErrorCallback(SD_HandleTypeDef* hsd) {
    uint32_t error = HAL_SD_GetError(hsd);
    HAL_SD_StateTypeDef state = HAL_SD_GetState(hsd);

    Log::error(fmt::format("SD error: {}, state: {}", error, state));
}
