#pragma once
#include <stdint.h>

/**
 * Font type
 */
typedef struct {
    const uint8_t width; /*!< Font width in pixels */
    uint8_t height; /*!< Font height in pixels */
    const uint16_t* data; /*!< Pointer to data font data array */
} Font;