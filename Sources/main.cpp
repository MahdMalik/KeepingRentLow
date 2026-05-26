#include <stdint.h>
#include <string.h>
#include "usb_setup.h"


// ==========================================
// MAIN EXECUTION SPACE
// ==========================================
int main(void) {
	HAL_Init();
	SystemClock_Config();
	SystemCoreClockUpdate();

	// Enable USB Core Hardware Clocks
	__HAL_RCC_USB_OTG_FS_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	init_usb_pins();

	tusb_init();

	SetupBlinkPin();

	uint32_t last_blink_time = 0;
	uint32_t last_tx_time = 0;

	for(;;) {
		tud_task(); // Keeps USB stack actively processing events

		uint32_t current_time = HAL_GetTick();

		// Non-blocking LED Blinky check every 500ms
		if (current_time - last_blink_time >= 500) {
			last_blink_time = current_time;
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		}

		// Transmit serial test message if terminal is connected
		if (tud_cdc_connected()) {
			if (current_time - last_tx_time >= 1000) {
				last_tx_time = current_time;
				tud_cdc_write_str("Hello World over TinyUSB!\r\n");
				tud_cdc_write_flush();
			}
		}
	}
}
