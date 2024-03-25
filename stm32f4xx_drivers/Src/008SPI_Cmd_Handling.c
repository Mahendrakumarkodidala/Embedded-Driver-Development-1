/*
 * 008SPI_Cmd_Handling.c
 *
 *  Created on: Feb 8, 2024
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

extern void initialise_monitor_handles(void);


//Arduino Command Codes
#define COMMAND_LED_CTRL			0x50
#define COMMAND_SENSOR_READ			0x51
#define COMMAND_LED_READ			0x52
#define COMMAND_PRINT				0x53
#define COMMAND_ID_READ				0x54

//Arduino LED Codes
#define LED_PIN		9
#define LED_ON		1
#define LED_OFF		0

//Arduino Analog Pins
#define ANALOG_PIN0		0
#define ANALOG_PIN1		1
#define ANALOG_PIN2		2
#define ANALOG_PIN3		3
#define ANALOG_PIN4		4
#define ANALOG_PIN5		5


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
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

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


uint8_t SPI_VerifyResponse(uint8_t ack_byte)
{
	if(ack_byte == 0xF5)
	{
		//ACK
		return 1;
	}
	else
	{
		//NACK
		return 0;
	}
}


int main(void)
{

	initialise_monitor_handles();

	printf("Application is Running\n");
	//Intialize the GPIO Button
	GPIO_BtnInits();

	//Intialize the GPIO pins to behave as SPI2 Pins
	SPI2_GPIOInits();

	//Intialize the SPI2 Pins
	SPI2_Inits();

	printf("SPI Initialized \n");

	//Enabling SSOE to enable NSS Output.
	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
		//When button is pressed, exit from this
		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

		//Enabling the SPI2 Peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//1. CMD_LED_CTRL
			uint8_t	commandcode = COMMAND_LED_CTRL;
			SPI_SendData(SPI2, &commandcode, 1);

			//Dummy read to clear RXNE Flag
			uint8_t dummy_read;
			SPI_ReceiveData(SPI2, &dummy_read, 1);

			//Sending dummy bits (1 byte) to fetch the response fron the slave
			uint8_t dummy_write = 0xff;
			SPI_SendData(SPI2, &dummy_write, 1);

			//Reading the ACK Response from the slave
			uint8_t ack_byte;
			SPI_ReceiveData(SPI2, &ack_byte, 1);


			uint8_t args[2];
			if(SPI_VerifyResponse(ack_byte))
			{
				//Send Command Arguments
				args[0] = LED_PIN;
				args[1] = LED_ON;
				SPI_SendData(SPI2, args, 2);

			}

		//When button is pressed, exit from this
		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

		//2. CMD_SENSOR_READ
			commandcode = COMMAND_SENSOR_READ;
			SPI_SendData(SPI2, &commandcode, 1);

			//Dummy read to clear RXNE Flag
			SPI_ReceiveData(SPI2, &dummy_read, 1);

			//Sending dummy bits (1 byte) to fetch the response fron the slave
			SPI_SendData(SPI2, &dummy_write, 1);

			//Reading the ACK Response from the slave
			SPI_ReceiveData(SPI2, &ack_byte, 1);

			if(SPI_VerifyResponse(ack_byte))
			{
				//Send Command Arguments
				args[0] = ANALOG_PIN0;
				SPI_SendData(SPI2, args, 1);

				//Dummy read to clear RXNE Flag
				SPI_ReceiveData(SPI2, &dummy_read, 1);

				//Inserting delay for slave can be ready with data
				delay();

				//Sending dummy bits (1 byte) to fetch the response fron the slave
				SPI_SendData(SPI2, &dummy_write, 1);

				//Read the data
				uint8_t analog_read;
				SPI_ReceiveData(SPI2, &analog_read, 1);

				printf("COMMAND_SENSOR_READ %d\n",analog_read);
			}

		//When button is pressed, exit from this
		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

		//3.CMD_LED_READ
			commandcode = COMMAND_LED_READ;
			SPI_SendData(SPI2, &commandcode, 1);

			//Dummy read to clear RXNE Flag
			SPI_ReceiveData(SPI2, &dummy_read, 1);

			//Sending dummy bits (1 byte) to fetch the response fron the slave
			SPI_SendData(SPI2, &dummy_write, 1);

			//Reading the ACK Response from the slave
			SPI_ReceiveData(SPI2, &ack_byte, 1);

			if(SPI_VerifyResponse(ack_byte))
			{
				//Send Command Arguments
				args[0] = LED_PIN;
				SPI_SendData(SPI2, args, 1);

				//Dummy read to clear RXNE Flag
				SPI_ReceiveData(SPI2, &dummy_read, 1);

				delay();

				//Sending dummy bits (1 byte) to fetch the response fron the slave
				SPI_SendData(SPI2, &dummy_write, 1);

				//Read the data
				uint8_t Led_data;
				SPI_ReceiveData(SPI2, &Led_data, 1);

				printf("COMMAND_READ_LED %d\n",Led_data);
			}

		//When button is pressed, exit from this
		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

			//4.CMD_PRINT
				commandcode = COMMAND_PRINT;
				SPI_SendData(SPI2, &commandcode, 1);

				//Dummy read to clear RXNE Flag
				SPI_ReceiveData(SPI2, &dummy_read, 1);

				//Sending dummy bits (1 byte) to fetch the response fron the slave
				SPI_SendData(SPI2, &dummy_write, 1);

				//Reading the ACK Response from the slave
				SPI_ReceiveData(SPI2, &ack_byte, 1);

				uint8_t message[] = "Hi";
				if(SPI_VerifyResponse(ack_byte))
				{
					//Send Command Arguments - Length
					args[0] = strlen((char *)message);
					SPI_SendData(SPI2, args, 1);

					//Dummy read to clear RXNE Flag
					SPI_ReceiveData(SPI2, &dummy_read, 1);

					delay();

					//Send Command Arguments - data
					for(int i=0; i < args[0]; i++)
					{
						SPI_SendData(SPI2, &message[i], 1);
						SPI_ReceiveData(SPI2, &dummy_read, 1);
					}

					printf("COMMAND_PRINT Executed\n");
				}

		//When button is pressed, exit from this
		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

			//5. CMD_ID_READ
						commandcode =  COMMAND_ID_READ;
						SPI_SendData(SPI2, &commandcode, 1);

						//Dummy read to clear RXNE Flag
						SPI_ReceiveData(SPI2, &dummy_read, 1);

						//Sending dummy bits (1 byte) to fetch the response fron the slave
						SPI_SendData(SPI2, &dummy_write, 1);

						//Reading the ACK Response from the slave
						SPI_ReceiveData(SPI2, &ack_byte, 1);

						uint8_t Id_data[11];

						if(SPI_VerifyResponse(ack_byte))
						{
							for(uint32_t i = 0; i < 10; i++)
							{
								//Sending dummy bits (1 byte) to fetch the response fron the slave
								SPI_SendData(SPI2, &dummy_write, 1);

								//Read the data
								SPI_ReceiveData(SPI2, Id_data, 1);
							}

							Id_data[10] = '\0';

							printf("COMMAND_ID = %s\n",Id_data);
						}


		//Checking whether the SPI is busy or not
		while(SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

		//Disabling the SPI2 Peripheral
		SPI_PeripheralControl(SPI2, DISABLE);

		printf("SPI Communication Closed\n");

	}

	return 0;
}
