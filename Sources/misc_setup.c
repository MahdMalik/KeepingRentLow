#include "misc_setup.h"

// Global Variables
uint32_t SystemCoreClock = 16000000;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

// ==========================================
// HARDWARE INITIALIZATION DEFINITIONS
// ==========================================

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 192;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_SYSCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_SYSCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

void SystemCoreClockUpdate(void) {
    // Since we're using PLL from HSE:
    // HSE = 25MHz, PLLM=25, PLLN=192, PLLP=2
    // VCO = 25 * (192/25) = 192MHz
    // SYSCLK = 192 / 2 = 96MHz
    SystemCoreClock = 96000000;
}

void SetupBlinkPin()
{
	GPIO_InitTypeDef GPIO_USB_InitStruct = {0};
	GPIO_USB_InitStruct.Pin = GPIO_PIN_13;
	GPIO_USB_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_USB_InitStruct.Pull = GPIO_NOPULL;
	GPIO_USB_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_USB_InitStruct);
}
