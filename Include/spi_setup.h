#pragma once

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx.h"
#include <string.h>
#include "usb_setup.h"
#include <stdio.h>
#include <stdbool.h>

#define SPI_SCK_PIN GPIO_PIN_5
#define SPI_MOSI_PIN GPIO_PIN_7
#define SPI_MISO_PIN GPIO_PIN_6
#define SPI_CS_PIN GPIO_PIN_4

#define SPI_PORT GPIOA

#ifdef __cplusplus
extern "C" {
#endif

void SetCsHigh();

void SetCsLow();

void SpiInit();

uint8_t SendTest();

void SendGarbage();

void ReadGarbage(int numberToRead);

void readGarbageUntilFirstBit(uint8_t* receivedBit);

bool SendResponseWithExpectedOutput(uint8_t* input, uint8_t* expectedOutput, uint8_t inputSize, uint8_t outputSize, bool printBits);

uint8_t SetupSdCard();

bool ReadBlock(uint32_t readAddr, uint8_t* outputBuffer);

#ifdef __cplusplus
}
#endif
