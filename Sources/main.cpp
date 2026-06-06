#include <stdint.h>
#include <string.h>
#include <cstdio>
#include "usb_setup.h"
#include "ff.h"
#include "diskio.h"
#include "spi_setup.h"
#include "misc_setup.h"
#include "sd_driver.h"

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

	while (!tud_cdc_connected())
	{
	    tud_task();  // must keep USB stack alive
	}

	SpiInit();

	char sdPath[4];
	FATFS sdFS;

	FATFS_LinkDriver(&SD_Driver, sdPath);

	FRESULT fr = f_mount(&sdFS, sdPath, 1);

	if (fr != FR_OK) {
	    PrintFunction(true, "Mount failed: %d\r\n", fr);
	    return -1;
	}

    PrintFunction(true, "Mount success!\r\n");
    FIL file;

    fr = f_open(&file, "SolemnLamentDing.wav", FA_READ);
    if(fr != FR_OK) {
        PrintFunction(true, "File open failed: %d\r\n", fr);
        return -1;
    }

    PrintFunction(true, "File opened successfully!\r\n");



	for(;;) {
		tud_task(); // Keeps USB stack actively processing events

		uint32_t current_time = HAL_GetTick();

		// Non-blocking LED Blinky check every 500ms
		if (current_time - last_blink_time >= 500) {
			last_blink_time = current_time;
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		}
	}
}
