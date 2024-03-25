/*
 * 013I2C_Slave_tx_string.c
 *
 *  Created on: Mar 2, 2024
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

#define SLAVE_ADDR  0X68
#define MY_ADDR		SLAVE_ADDR

uint8_t Tx_buf[32] = "Hi, This is Mahendra";

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

	//I2C IRQ Configurations
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	I2c_SlaveEnableDisableCallbackEvents(I2C1,ENABLE);

	//Enable the I2C Peripheral
	I2C_PeriClockControl(I2C1,ENABLE);

	//ACK bit is enabled after PE=1
	I2C_ManageAcking(I2C1,I2C_ACK_ENABLE);


	while(1);

}

void I2C1_EV_IRQHandler(void)
{
	 // I2C1 event interrupt
	I2C_EV_IRQHandling(&I2C1handle);

}
void I2C1_ER_IRQHandler(void)
{
	// I2C1 error interrupt
	I2C_ER_IRQHandling(&I2C1handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
	static uint8_t commandcode = 0;
	static uint8_t count = 0;

	if(AppEv == I2C_EV_DATA_REQ)
	{
		// Master require data, slave -> send
		if(commandcode == 0x51)
		{
			//Send Len
			I2C_SlaveSendData(pI2CHandle->pI2Cx, strlen((char*)Tx_buf));
		}else if(commandcode == 0x52)
		{
			//Send contents of Tx_buf
			I2C_SlaveSendData(pI2CHandle->pI2Cx, Tx_buf[count++]);
		}

	}else if(AppEv == I2C_EV_DATA_RCV)
	{
		// slave -> Receive
		commandcode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);

	}else if(AppEv == I2C_ERROR_AF)
	{
		// Happens in Slave TX, master -> NACK
		commandcode = 0xff;
		count = 0;

	}else if(AppEv == I2C_EV_STOP)
	{
		// Happens in Slave RX, master ended slave communication
		count = 0;
	}
}


