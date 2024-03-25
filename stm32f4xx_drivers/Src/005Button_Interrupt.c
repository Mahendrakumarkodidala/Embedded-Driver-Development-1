/*
 * 005Button_Interrupt.c
 *
 *  Created on: Feb 4, 2024
 *      Author: mahen
 */
#include <stdint.h>

#include <string.h>

#include "stm32f407xx.h"

void delay(void)
{
	for (int i = 0; i< 500000; i++);
}

int main(void)
{
	GPIO_Handle_t GPIOLed,GPIOBtn;
	memset(&GPIOLed,0,sizeof(GPIOLed));
	memset(&GPIOBtn,0,sizeof(GPIOBtn));


	//GPIO LED Configuration
	GPIOLed.pGPIOx = GPIOD;
	GPIOLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIOLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD , ENABLE);
	GPIO_Init(&GPIOLed);

	//GPIO Button Configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GPIOBtn);

	//IRQ Configurations
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);

	//IRQ Priority
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PRI15);

	while(1);
}

void EXTI0_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}

