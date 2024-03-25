/*
 * stm32f407xx_rcc_driver.c
 *
 *  Created on: Mar 3, 2024
 *      Author: mahen
 */
#include "stm32f407xx.h"

uint16_t AHB_Prescalar[8] = {2,4,8,16,64,128,256,512};

uint16_t APB1_Prescalar[4] = {2,4,8,16};

uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1,SystemClk;
	uint8_t clksrc,temp,apb1p;
	uint16_t ahbp;

	clksrc = ((RCC->CFGR >> 2) & 0x3);

	if(clksrc == 0)
	{
		SystemClk = 16000000;
	}else if(clksrc == 1)
	{
		SystemClk = 8000000;
	}else if(clksrc == 2)
	{
		SystemClk = RCC_GetPLLOutputClock();
	}

	//AHB
	temp = ((RCC->CFGR >> 4) & 0xF);

	if(temp < 8)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_Prescalar[temp-8];
	}

	//APB
	temp = ((RCC->CFGR >> 10) & 0x7);

	if(temp < 4)
	{
		apb1p = 1;
	}else
	{
		apb1p = APB1_Prescalar[temp-4];
	}

	pclk1 = (SystemClk/ahbp)/apb1p;

	return pclk1;
}

uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t SystemClock=0,temp,pclk2;
	uint8_t clk_src = ( RCC->CFGR >> 2) & 0X3;

	uint8_t ahbp,apb2p;

	if(clk_src == 0)
	{
		SystemClock = 16000000;
	}else
	{
		SystemClock = 8000000;
	}
	temp = (RCC->CFGR >> 4 ) & 0xF;

	if(temp < 0x08)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_Prescalar[temp-8];
	}

	temp = (RCC->CFGR >> 13 ) & 0x7;
	if(temp < 0x04)
	{
		apb2p = 1;
	}else
	{
		apb2p = APB1_Prescalar[temp-4];
	}

	pclk2 = (SystemClock / ahbp )/ apb2p;

	return pclk2;
}

uint32_t  RCC_GetPLLOutputClock(void)
{

	return 0;
}
