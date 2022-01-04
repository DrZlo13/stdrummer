#include "gfx.h"
#include "display/ssd1306-i2c.h"

#define GFX_SPEC GFX<SSD1306_I2C<128, 64>>

using Color = GFX_SPEC::Color;
extern GFX_SPEC gfx;