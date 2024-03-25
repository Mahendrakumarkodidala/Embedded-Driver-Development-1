/*
 * 006SPI_Tx_Testing.c
 *
 *  Created on: Feb 6, 2024
 *      Author: mahen
 */


/*
 * SPI2 GPIO Pins
 * PB14 -> SPI2_MISO
 * PB15 -> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 -> SPI2_NSS
 * Alternate Function Mode -> 5
 */
#include <stdint.h>

#include <string.h>

#include "stm32f407xx.h"

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;
	memset(&SPIPins,0,sizeof(SPIPins));

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;

	//1. Intializing PB14 -> SPI2_MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

	//2. Intializing PB15 -> SPI2_MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//3. Intializing PB13 -> SPI2_SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//4. Intializing PB12 -> SPI2_NSS
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	//GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{
	SPI_Handle_t SPI2handle;
	memset(&SPI2handle,0,sizeof(SPI2handle));

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SCLKSpeed = SPI_SCLK_Speed_DIV2;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;


	SPI_Init(&SPI2handle);
}


int main(void)
{
	char user_data[] = "Hello World";
	//Intialize the GPIO pins to behave as SPI2 Pins
	SPI2_GPIOInits();

	//Intialize the SPI2 Pins
	SPI2_Inits();

	//Enabling the SSI bit to make NSS signal internally high & avoid MODF error
	SPI_SSIConfig(SPI2, ENABLE);

	//Enabling the SPI2 Peripheral
	SPI_PeripheralControl(SPI2, ENABLE);

	//Sending the data
	SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

	//Checking whether the SPI is busy or not
	while(SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

	//Disabling the SPI2 Peripheral
	SPI_PeripheralControl(SPI2, DISABLE);

	while(1);

	return 0;
}

