#include "spi_setup.h"

SPI_HandleTypeDef spi_handle;

void SetCsHigh()
{
	HAL_GPIO_WritePin(SPI_PORT, SPI_CS_PIN, GPIO_PIN_SET);
}

void SetCsLow()
{
	HAL_GPIO_WritePin(SPI_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
}

void SpiInit()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_SPI_InitStruct = {0};
	GPIO_SPI_InitStruct.Pin = SPI_SCK_PIN | SPI_MISO_PIN | SPI_MOSI_PIN;
	GPIO_SPI_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_SPI_InitStruct.Pull = GPIO_NOPULL;
	GPIO_SPI_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_SPI_InitStruct.Alternate = GPIO_AF5_SPI1;

	HAL_GPIO_Init(SPI_PORT, &GPIO_SPI_InitStruct);

	memset(&GPIO_SPI_InitStruct, 0, sizeof(GPIO_SPI_InitStruct));

	GPIO_SPI_InitStruct.Pin = SPI_CS_PIN;
	GPIO_SPI_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_SPI_InitStruct.Pull = GPIO_PULLUP;
	GPIO_SPI_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(SPI_PORT, &GPIO_SPI_InitStruct);

	SetCsHigh();

	spi_handle.Instance = SPI1;
	spi_handle.Init.Mode = SPI_MODE_MASTER;
	spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
	spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
	spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi_handle.Init.NSS = SPI_NSS_SOFT;
	spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
	spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

	HAL_SPI_Init(&spi_handle);
}

uint8_t SendTest()
{
	uint8_t tx = 0xAA;
	uint8_t rx = 0;

	SetCsLow();
	HAL_SPI_TransmitReceive(&spi_handle, &tx, &rx, 1, 100);
	SetCsHigh();

	return rx;
}

void SendGarbage()
{
	uint8_t dummy = 0xFF;
	for(int i = 0; i < 10; i++) HAL_SPI_Transmit(&spi_handle, &dummy, 1, 100);
}

void ReadGarbage(int numberToRead)
{
	uint8_t dummyBuffer = 0xFF;
	for(int i = 0; i < numberToRead; i++) HAL_SPI_Receive(&spi_handle, &dummyBuffer, 1, 100);
}

void ReadGarbageUntilFirstBit(uint8_t* receivedBit)
{
	for(int i = 0; i < 8; i++)
	{
		HAL_SPI_Receive(&spi_handle, receivedBit, 1, 100);
		if(*receivedBit != 0xFF)
		{
			break;
		}
	}
}

void WaitForTokenOnTimeout(uint8_t* receivedBit)
{
	uint32_t readStart = HAL_GetTick();
	while(HAL_GetTick() - readStart < 200)
	{
		HAL_SPI_Receive(&spi_handle, receivedBit, 1, 1);
		if(*receivedBit != 0xFF)
		{
			break;
		}

	}
}

bool SendResponseWithExpectedOutput(uint8_t* input, uint8_t* expectedOutput, uint8_t inputSize, uint8_t outputSize, bool printBits)
{
	SendGarbage();

	SetCsLow();
	HAL_SPI_Transmit(&spi_handle, input, inputSize, 100);
	uint8_t response = 0xFF;

	ReadGarbageUntilFirstBit(&response);

	if(printBits) PrintFunction(false, "Bits Received Are: %02X, ", response);

	if(response == 0xFF || response != expectedOutput[0])
	{
		if(printBits) PrintFunction(true, "\n\r");
		SetCsHigh();
		return false;
	}


	for(int i = 1; i < outputSize; i++)
	{
		HAL_SPI_Receive(&spi_handle, &response, 1, 100);

		if(printBits) PrintFunction(false, "%02X, ", response);

		if(response != expectedOutput[i])
		{
			if(printBits) PrintFunction(true, "\n\r");
			SetCsHigh();
			return false;
		}
	}

	SetCsHigh();

	if(printBits) PrintFunction(true, "\n\r");

	return true;
}

uint8_t SetupSdCard()
{
	SendGarbage();

	uint8_t cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
	uint8_t cmd0Output[] = {0x01};

	if(!SendResponseWithExpectedOutput(cmd0, cmd0Output, sizeof(cmd0), sizeof(cmd0Output), false)) return 0xFF;

	uint8_t cmd8[] = {0x48, 0x00, 0x00, 0x01, 0xAA, 0x87};
	uint8_t cmd8Output[] = {0x01, 0x00, 0x00, 0x01, 0xAA};

	if(!SendResponseWithExpectedOutput(cmd8, cmd8Output, sizeof(cmd8), sizeof(cmd8Output), false)) return 0xFF;


	uint8_t cmd55[] = {0x77, 0x00, 0x00, 0x00, 0x00, 0x01};
	uint8_t cmd55Output[] = {0x01};

	uint8_t acmd41[] = {0x69, 0x40, 0x00, 0x00, 0x00, 0x01};
	uint8_t acmd41Output[] = {0x00};

	uint32_t start_time = HAL_GetTick();
	while(HAL_GetTick() - start_time <= 2000)
	{
		tud_task();
		if(!SendResponseWithExpectedOutput(cmd55, cmd55Output, sizeof(cmd55), sizeof(cmd55Output), false)) return 0xFF;

		if(SendResponseWithExpectedOutput(acmd41, acmd41Output, sizeof(acmd41), sizeof(acmd41Output), false))
		{
			break;
		}

		HAL_Delay(10);
	}

	uint8_t cmd58[] = {0x7A, 0x00, 0x00, 0x00, 0x00, 0x01};
	uint8_t cmd58Output[] = {0x00, 0xC0, 0xFF, 0x80, 0x00};

	if(!SendResponseWithExpectedOutput(cmd58, cmd58Output, sizeof(cmd58), sizeof(cmd58Output), false)) return 0xFF;

	uint8_t cmd16[] = {0x50, 0x00, 0x00, 0x02, 0x00, 0x01};
	uint8_t cmd16Output[] = {0x00};
	if(!SendResponseWithExpectedOutput(cmd16, cmd16Output, sizeof(cmd16), sizeof(cmd16Output), false)) return 0xFF;

	for(int i = 0; i < 25; i++)
	{
		HAL_Delay(10);
		tud_task();
	}

	return 0x05;
}

bool ReadBlock(uint32_t readAddr, uint8_t* outputBuffer)
{
	uint8_t cmd17[] = {0x51, (readAddr >> 24) & 0xFF,
			(readAddr >> 16) & 0xFF, (readAddr >> 8) & 0xFF,
			readAddr & 0xFF, 0x01};


	SendGarbage();
	SetCsLow();

	HAL_SPI_Transmit(&spi_handle, cmd17, 6, 100);

	// first get the r1 bit, 0x00
	uint8_t r1Bit = 0xFF;
	ReadGarbageUntilFirstBit(&r1Bit);
	if(r1Bit != 0x00)
	{
		SetCsHigh();
		PrintFunction(true, "R1 failed, instead got %02X!\r\n", r1Bit);
		return false;
	}
	else
	{
//		PrintFunction(true, "R1 passed.\r\n");
	}

	uint8_t readyToken = 0xFF;
	WaitForTokenOnTimeout(&readyToken);
	if(readyToken != 0xFE)
	{
		SetCsHigh();
		PrintFunction(true, "Ready token failed!\r\n");
		return false;
	}
	else
	{
//		PrintFunction(true, "ready token passed.\r\n");
	}


	// read dem bytes
	HAL_SPI_Receive(&spi_handle, outputBuffer, 512, 200);


	ReadGarbage(2);

	SetCsHigh();

	ReadGarbage(1);

	return true;
}
