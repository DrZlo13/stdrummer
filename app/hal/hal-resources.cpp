#include "hal-resources.h"

extern I2S_HandleTypeDef hi2s1;
extern TIM_HandleTypeDef htim4;

HalGpio gpio_led(LED_PIN_GPIO_Port, LED_PIN_Pin);
HalGpio gpio_button(GPIOA, LL_GPIO_PIN_0);
HalUart uart_debug(USART1);
HalI2S i2s_dac(&hi2s1);
HalSD storage;
HalEncoder encoder(&htim4);
HalGpio encoder_button(GPIOC, LL_GPIO_PIN_15);