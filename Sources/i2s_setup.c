#include "i2s_setup.h"

static I2S_HandleTypeDef i2s_handle;

void I2sInit()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_SPI2_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_I2S_InitStruct = {0};

    GPIO_I2S_InitStruct.Pin = I2S_BCLK_PIN | I2S_LRC_PIN | I2S_DIN_PIN;
    GPIO_I2S_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_I2S_InitStruct.Pull = GPIO_NOPULL;
    GPIO_I2S_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_I2S_InitStruct.Alternate = GPIO_AF5_SPI2;

    HAL_GPIO_Init(I2S_PORT, &GPIO_I2S_InitStruct);

    memset(&GPIO_I2S_InitStruct, 0, sizeof(GPIO_InitTypeDef));

    GPIO_I2S_InitStruct.Pin = I2S_SD_PIN | I2S_GAIN_PIN;
    GPIO_I2S_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_I2S_InitStruct.Alternate = 0;

    HAL_GPIO_Init(I2S_PORT, &GPIO_I2S_InitStruct);

    HAL_GPIO_WritePin(I2S_PORT, I2S_SD_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2S_PORT, I2S_GAIN_PIN, GPIO_PIN_RESET);

    i2s_handle.Instance = SPI2;
    i2s_handle.Init.Mode = I2S_MODE_MASTER_TX;

    i2s_handle.Init.Standard = I2S_STANDARD_PHILIPS;
    i2s_handle.Init.DataFormat = I2S_DATAFORMAT_16B;
    i2s_handle.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    i2s_handle.Init.AudioFreq = 22050;
    i2s_handle.Init.CPOL = I2S_CPOL_LOW;
    i2s_handle.Init.ClockSource = I2S_CLOCK_PLL;
    i2s_handle.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;

    if(HAL_I2S_Init(&i2s_handle) != HAL_OK)
    {
    	PrintFunction(true, "Well, init failed lol.\r\n");
    }
    else
    {
    	PrintFunction(true, "I2S successfully set up.\r\n");
    }
}
