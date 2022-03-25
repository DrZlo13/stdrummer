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

    BSP_SD_CardInfo CardInfo;
    BSP_SD_GetCardInfo(&CardInfo);

    Log::info(
        "BSP Card info: {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x}",
        CardInfo.CardType,
        CardInfo.CardVersion,
        CardInfo.Class,
        CardInfo.RelCardAdd,
        CardInfo.BlockNbr,
        CardInfo.BlockSize,
        CardInfo.LogBlockNbr,
        CardInfo.LogBlockSize);
}

#define CASE_RET_STR(x) \
    case x:             \
        return #x;

static const char* sd_error_text(uint32_t error) {
    switch(error) {
        CASE_RET_STR(HAL_SD_ERROR_NONE);
        CASE_RET_STR(HAL_SD_ERROR_CMD_CRC_FAIL);
        CASE_RET_STR(HAL_SD_ERROR_DATA_CRC_FAIL);
        CASE_RET_STR(HAL_SD_ERROR_CMD_RSP_TIMEOUT);
        CASE_RET_STR(HAL_SD_ERROR_DATA_TIMEOUT);
        CASE_RET_STR(HAL_SD_ERROR_TX_UNDERRUN);
        CASE_RET_STR(HAL_SD_ERROR_RX_OVERRUN);
        CASE_RET_STR(HAL_SD_ERROR_ADDR_MISALIGNED);
        CASE_RET_STR(HAL_SD_ERROR_BLOCK_LEN_ERR);
        CASE_RET_STR(HAL_SD_ERROR_ERASE_SEQ_ERR);
        CASE_RET_STR(HAL_SD_ERROR_BAD_ERASE_PARAM);
        CASE_RET_STR(HAL_SD_ERROR_WRITE_PROT_VIOLATION);
        CASE_RET_STR(HAL_SD_ERROR_LOCK_UNLOCK_FAILED);
        CASE_RET_STR(HAL_SD_ERROR_COM_CRC_FAILED);
        CASE_RET_STR(HAL_SD_ERROR_ILLEGAL_CMD);
        CASE_RET_STR(HAL_SD_ERROR_CARD_ECC_FAILED);
        CASE_RET_STR(HAL_SD_ERROR_CC_ERR);
        CASE_RET_STR(HAL_SD_ERROR_GENERAL_UNKNOWN_ERR);
        CASE_RET_STR(HAL_SD_ERROR_STREAM_READ_UNDERRUN);
        CASE_RET_STR(HAL_SD_ERROR_STREAM_WRITE_OVERRUN);
        CASE_RET_STR(HAL_SD_ERROR_CID_CSD_OVERWRITE);
        CASE_RET_STR(HAL_SD_ERROR_WP_ERASE_SKIP);
        CASE_RET_STR(HAL_SD_ERROR_CARD_ECC_DISABLED);
        CASE_RET_STR(HAL_SD_ERROR_ERASE_RESET);
        CASE_RET_STR(HAL_SD_ERROR_AKE_SEQ_ERR);
        CASE_RET_STR(HAL_SD_ERROR_INVALID_VOLTRANGE);
        CASE_RET_STR(HAL_SD_ERROR_ADDR_OUT_OF_RANGE);
        CASE_RET_STR(HAL_SD_ERROR_REQUEST_NOT_APPLICABLE);
        CASE_RET_STR(HAL_SD_ERROR_PARAM);
        CASE_RET_STR(HAL_SD_ERROR_UNSUPPORTED_FEATURE);
        CASE_RET_STR(HAL_SD_ERROR_BUSY);
        CASE_RET_STR(HAL_SD_ERROR_DMA);
        CASE_RET_STR(HAL_SD_ERROR_TIMEOUT);
#if defined(USE_HAL_SD_REGISTER_CALLBACKS) && (USE_HAL_SD_REGISTER_CALLBACKS == 1U)
        CASE_RET_STR(HAL_SD_ERROR_INVALID_CALLBACK);
#endif

    default:
        return "Unknown error";
    }
}

static const char* sd_state_text(HAL_SD_StateTypeDef state) {
    switch(state) {
        CASE_RET_STR(HAL_SD_STATE_RESET);
        CASE_RET_STR(HAL_SD_STATE_READY);
        CASE_RET_STR(HAL_SD_STATE_TIMEOUT);
        CASE_RET_STR(HAL_SD_STATE_BUSY);
        CASE_RET_STR(HAL_SD_STATE_PROGRAMMING);
        CASE_RET_STR(HAL_SD_STATE_RECEIVING);
        CASE_RET_STR(HAL_SD_STATE_TRANSFER);
        CASE_RET_STR(HAL_SD_STATE_ERROR);
    default:
        return "Unknown state";
    }
}

extern "C" void BSP_SD_AbortCallback(void) {
    Log::error("SD Abort");
}

extern "C" void HAL_SD_ErrorCallback(SD_HandleTypeDef* hsd) {
    uint32_t error = HAL_SD_GetError(hsd);
    HAL_SD_StateTypeDef state = HAL_SD_GetState(hsd);

    Log::error(sd_state_text(state));
    Core::crash(sd_error_text(error));
}
