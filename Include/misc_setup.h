#pragma once

#include "../Libraries/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);
void SystemCoreClockUpdate(void);
void SetupBlinkPin();

#ifdef __cplusplus
}
#endif
