#include "hal-resources.h"

extern I2S_HandleTypeDef hi2s1;

HalGpio gpio_led(GPIOC, LL_GPIO_PIN_13);
HalGpio gpio_button(GPIOA, LL_GPIO_PIN_0);
HalUart uart_debug(USART1);
HalI2S i2s_dac(&hi2s1);