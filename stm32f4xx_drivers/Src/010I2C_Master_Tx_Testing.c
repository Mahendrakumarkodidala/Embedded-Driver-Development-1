/*
 * 010I2C_Master_Tx_Testing.c
 *
 *  Created on: Feb 18, 2024
 *      Author: mahen
 */
#include <stdio.h>

#include <stdint.h>

#include "stm32f407xx.h"

#include <string.h>

/*
 * PB6 - SCL
 * PB7 - SDA
 */

void delay(void)
{
	for (int i = 0; i< 500000/2; i++);
}

I2C_Handle_t I2C1handle;

#define MY_ADDR 	0X61
#define SLAVE_ADDR  0X68

uint8_t some_data[] = "We are testing I2C master Tx\n";

void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;
	memset(&I2CPins,0,sizeof(I2CPins));

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;

	//1. Intializing PB6 - SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	//2. Intializing PB9 - SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);
}

void I2C1_Inits(void)
{
	memset(&I2C1handle,0,sizeof(I2C1handle));

	I2C1handle.pI2Cx = I2C1;
	I2C1handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1handle.I2C_Config.I2C_DeviceAddress = MY_ADDR ;
	I2C1handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1handle);
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
	//GPIO Button Initialization
	GPIO_BtnInits();

	//I2C Pin Intialization
	I2C1_GPIOInits();

	//I2C Peripheral Intialization
	I2C1_Inits();

	//Enable the I2C Peripheral
	I2C_PeriClockControl(I2C1,ENABLE);

	while(1)
	{
		//When button is pressed, exit from this
		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		//Delay to avoid Button De-bouncing
		delay();

		//Sending some data to Slave
		I2C_MasterSendData(&I2C1handle, some_data, strlen((char*)some_data), SLAVE_ADDR);

	}
	return 0;

}


