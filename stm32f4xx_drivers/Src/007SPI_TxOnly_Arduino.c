/*
 * 007SPI_TxOnly_Arduino.c
 *
 *  Created on: Feb 7, 2024
 *      Author: mahen
 */

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

/* SPI Slave Demo
 *
 * SPI pin numbers:
 * SCK   13  // Serial Clock.
 * MISO  12  // Master In Slave Out.
 * MOSI  11  // Master Out Slave In.
 * SS    10  // Slave Select . Arduino SPI pins respond only if SS pulled low by the master
 *
 */
#include <stdio.h>

#include "stm32f407xx.h"

#include <string.h>

void delay(void)
{
	for (int i = 0; i< 500000/2; i++);
}

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

	//1. Intializing PB13 -> SPI2_SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//2. Intializing PB15 -> SPI2_MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//3. Intializing PB14 -> SPI2_MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

	//4. Intializing PB12 -> SPI2_NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{
	SPI_Handle_t SPI2handle;
	memset(&SPI2handle,0,sizeof(SPI2handle));

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SCLKSpeed = SPI_SCLK_Speed_DIV8;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;			//Hardware


	SPI_Init(&SPI2handle);
}

void GPIO_BtnInits(void)
{
	GPIO_Handle_t GPIOBtn;
	memset(&GPIOBtn,0,sizeof(GPIOBtn));

	//GPIO Button Configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);
}

int main(void)
{
	char user_data[] = "Hello World";

	//Intialize the GPIO Button
	GPIO_BtnInits();

	//Intialize the GPIO pins to behave as SPI2 Pins
	SPI2_GPIOInits();

	//Intialize the SPI2 Pins
	SPI2_Inits();

	//Enabling SSOE to enable NSS Output.
	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
		//When button is pressed, exit from this
		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

		//Enabling the SPI2 Peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//Sending the Data Length
		uint8_t DataLen = strlen(user_data);
		SPI_SendData(SPI2, &DataLen, 1);

		//Sending the data
		SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

		//Checking whether the SPI is busy or not
		while(SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

		//Disabling the SPI2 Peripheral
		SPI_PeripheralControl(SPI2, DISABLE);

	}

	return 0;
}

