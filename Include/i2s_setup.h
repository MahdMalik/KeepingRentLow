#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2s.h"
#include "usb_setup.h"

#define I2S_BCLK_PIN GPIO_PIN_13
#define I2S_LRC_PIN GPIO_PIN_12
#define I2S_DIN_PIN GPIO_PIN_15
#define I2S_SD_PIN GPIO_PIN_6
#define I2S_GAIN_PIN GPIO_PIN_7

#define I2S_PORT GPIOB

#ifdef __cplusplus
extern "C" {
#endif

void I2sInit();

#ifdef __cplusplus
}
#endif

