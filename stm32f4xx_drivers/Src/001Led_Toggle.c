/*
 * 001Led_Toggle.c
 *
 *  Created on: Feb 2, 2024
 *      Author: mahen
 */
#include <stdint.h>

#include "stm32f407xx.h"



void delay(void)
{
	for (int i = 0; i< 500000; i++);
}
int main(void)
{
	GPIO_Handle_t GPIOLed;

	GPIOLed.pGPIOx = GPIOD;
	GPIOLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIOLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD , ENABLE);
	GPIO_Init(&GPIOLed);

	while(1)
	{
		//GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_12, 0); //Turnoff LED

		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		delay();
	}





	return 0;

}

