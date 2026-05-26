#pragma once

#include "../Libraries/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h"
#include "../Libraries/TinyUSB/tusb.h"

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)
#define EPNUM_CDC_NOTIF   0x81
#define EPNUM_CDC_OUT     0x02
#define EPNUM_CDC_IN      0x82


#ifdef __cplusplus
extern "C" {
#endif
void init_usb_pins(void);

void SysTick_Handler(void);

void OTG_FS_IRQHandler(void);

uint32_t tusb_time_millis_api(void);

uint8_t const * tud_descriptor_device_cb(void);

uint8_t const * tud_descriptor_configuration_cb(uint8_t index);

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid);

void SystemClock_Config(void);

void SetupBlinkPin();

#ifdef __cplusplus
}
#endif
