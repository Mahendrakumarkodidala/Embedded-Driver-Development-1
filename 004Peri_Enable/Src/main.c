/*
 * main.c
 *
 *  Created on: Jan 29, 2024
 *      Author: mahen
 */
#include <stdint.h>

#define RCC_APB2ENR_REG 		0x40023800UL

#define RCC_APB2ENR_REG_OFF 	0x44UL

#define RCC_APB2ENR				(RCC_APB2ENR_REG + RCC_APB2ENR_REG_OFF)

#define ADC_CR1_REG				0x40012000UL

#define	ADC_CR1_OFF				0x04UL

#define ADC_CR1					(ADC_CR1_REG + ADC_CR1_OFF)

int main()
{
	uint32_t *pRccApb2En = (uint32_t*)RCC_APB2ENR;

	*pRccApb2En = *pRccApb2En | (1 <<8);

	uint32_t *pAdcCr1 = (uint32_t*)ADC_CR1;

	*pAdcCr1 = *pAdcCr1| (1 <<8);


	for(;;);
}

