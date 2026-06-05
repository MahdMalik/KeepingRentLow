#include <stdint.h>
#include <string.h>
#include <cstdio>
#include "usb_setup.h"
#include "ff.h"
#include "diskio.h"
#include "spi_setup.h"
#include "misc_setup.h"

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

	while (!tud_cdc_connected())
	{
	    tud_task();  // must keep USB stack alive
	}

	SpiInit();

	uint8_t result = SetupSdCard();

	PrintFunction(true, "Result was: %02X\r\n", result);

	HAL_Delay(10);

	uint8_t readData[512];
	if(ReadBlock(0, readData))
	{
		PrintFunction(true, "Read data successfully!\r\n");
	}
	else
	{
		PrintFunction(true, "Read data failed!\r\n");
	}

	for(;;) {
		tud_task(); // Keeps USB stack actively processing events

		uint32_t current_time = HAL_GetTick();

		// Non-blocking LED Blinky check every 500ms
		if (current_time - last_blink_time >= 500) {
			last_blink_time = current_time;
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		}

		if(current_time - last_tx_time >= 1000)
		{
			last_tx_time = current_time;
			uint8_t returnedData = SendTest();
			PrintFunction(true, "Next bit read is this: %02X\r\n", returnedData);
		}
	}
}
