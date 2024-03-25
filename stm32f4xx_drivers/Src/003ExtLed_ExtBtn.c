/*
 * 003ExtLed_ExtBtn.c
 *
 *  Created on: Feb 2, 2024
 *      Author: mahen
 */
#include <stdint.h>

#include "stm32f407xx.h"


#define Btn_Press HIGH

void delay(void)
{
	for (uint32_t i = 0; i< 500000/2; i++);
}
int main(void)
{
	GPIO_Handle_t GPIOLed, GPIOBtn;

	//GPIO LED Configuration
	GPIOLed.pGPIOx = GPIOA;
	GPIOLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	GPIOLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GPIOLed);

	//GPIO Button Configuration
	GPIOBtn.pGPIOx = GPIOB;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOB, ENABLE);
	GPIO_Init(&GPIOBtn);

	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_12) != Btn_Press) //BUTTON PRESS IS LOW, in this case.
		{
			delay();
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_14);
		}
	}

	return 0;

}

