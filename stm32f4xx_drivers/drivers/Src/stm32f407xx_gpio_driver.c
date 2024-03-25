/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Jan 31, 2024
 *      Author: mahen
 */

#include "stm32f407xx_gpio_driver.h"

/************************************Peripheral Clock setup********************************************************/


/********************************************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the GPIO peripheral
 * @param[in]         - ENABLE or DISABLE macros
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}

		if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}

		if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}

		if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}

		if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}

		if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}

		if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}

		if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}

		if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}

		if(pGPIOx == GPIOJ)
		{
			GPIOJ_PCLK_EN();
		}

		if(pGPIOx == GPIOK)
		{
			GPIOK_PCLK_EN();
		}

	}else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}

		if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}

		if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}

		if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}

		if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}

		if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}

		if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}

		if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}

		if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}

		if(pGPIOx == GPIOJ)
		{
			GPIOJ_PCLK_DI();
		}

		if(pGPIOx == GPIOK)
		{
			GPIOK_PCLK_DI();
		}
	}
}


/************************************Init and De-Init********************************************************/

/********************************************************************************************
 * @fn      		  - GPIO_Init
 *
 * @brief             - This function Initializes the given GPIO port
 *
 * @param[in]         - Pointer to GPIO handle structure that contains GPIO port base address of the GPIO
 * 						peripheral and pin Configurations.
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	// Enable Peripheral Clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	uint32_t temp = 0; //Temparorary Register

	//1. Configure the Mode of GPIO Pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//Non-Interrupt Mode
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER |= temp;
	}else
	{
		//Interrupt Mode
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//1. Configure the FTSR
			EXTI->FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//1. Configure the RTSR & Clear FTSR corresponding bit
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//1. Configure both the FTSR & RTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. Configure GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] &= ~(0xF << (temp2 * 4));
		SYSCFG->EXTICR[temp1] |= portcode << (temp2 * 4);

		//3. Enable the EXTI interrupt delivery using IMR
		EXTI->IMR |=  (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	}

	temp = 0;

	//2. Configure the speed of output of GPIO Pin
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	temp = 0;

	//3. Configure the PUPD Settings of GPIO Pin
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp = 0;

	//4. Configure the Output type of GPIO Pin
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (1 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << (1 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	temp = 0;

	//5. Configure the Alternate functionality of GPIO Pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint32_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1]&= ~(0xF << (4 * (temp2)));
		pGPIOHandle->pGPIOx->AFR[temp1]|= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * (temp2)));

	}
}


/********************************************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             - This function Deinitializes the given GPIO port
 *
 * @param[in]         - base address of the GPIO peripheral
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}

	if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}

	if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}

	if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}

	if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}

	if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}

	if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}

	if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}

	if(pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}

	if(pGPIOx == GPIOJ)
	{
		GPIOJ_REG_RESET();
	}

	if(pGPIOx == GPIOK)
	{
		GPIOK_REG_RESET();
	}

}


/************************************Data read and write********************************************************/

/********************************************************************************************
 * @fn      		  - GPIO_ReadFromInputPin
 *
 * @brief             - This function reads the data from the given GPIO pin of GPIO port
 *
 * @param[in]         - base address of the GPIO peripheral
 * @param[in]		  - GPIO pin number
 *
 * @return            -  Data from pin [0 or 1]
 *
 * @Note              -  none

 *********************************************************************************************/

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
   uint8_t value;

   value = (uint8_t )((pGPIOx->IDR  >> PinNumber) & 0x00000001 ) ;

   return value;
}

/********************************************************************************************
 * @fn      		  - GPIO_ReadFromInputPort
 *
 * @brief             - This function reads the data from the given GPIO port
 *
 * @param[in]         - base address of the GPIO peripheral
 * @param[in]		  - GPIO pin number
 *
 * @return            -  Data from port
 *
 * @Note              -  none

 *********************************************************************************************/
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;

	return value;
}


/********************************************************************************************
 * @fn      		  - GPIO_WriteToOutputPin
 *
 * @brief             - This function writes the data to the given GPIO pin of GPIO port
 *
 * @param[in]         - base address of the GPIO peripheral
 * @param[in]		  - GPIO pin number
 * @param[in]		  -	Data
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		//Write 1 to the output data register corresponding to the pin number.
		//REG |= (0 << PinNum); will have no effect on REG. Verify using boolean algebra.
		pGPIOx->ODR |= (1 << PinNumber);
	}else
	{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}


/********************************************************************************************
 * @fn      		  - GPIO_WriteToOutputPort
 *
 * @brief             - This function writes the data to the given GPIO port
 *
 * @param[in]         - base address of the GPIO peripheral
 * @param[in]		  -	Data
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value )
{
	pGPIOx->ODR = Value;
}

/********************************************************************************************
 * @fn      		  - GPIO_ToggleOutputPin
 *
 * @brief             - This function toggles the output of the given GPIO pin of GPIO port
 *
 * @param[in]         - base address of the GPIO peripheral
 * @param[in]		  - GPIO pin number
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber );
}


/************************************IRQ Configuration and ISR handling***************************************************/

/********************************************************************************************
 * @fn      		  - GPIO_IRQConfig
 *
 * @brief             - This function configures the IRQ from the user
 *
 * @param[in]         - IRQ Number
 * @param[in]		  - IRQ priority
 * @param[in]		  -	ENABLE or DISABLE macros
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//Configure NVIC_ISER0 - 0 to 31
			*NVIC_ISER0 |= (1 << IRQNumber);
		}
		else if (IRQNumber > 31 && IRQNumber < 64)
		{
			//Configure NVIC_ISER1 - 31 to 63
			*NVIC_ISER1 |= (1 << (IRQNumber % 32) );
		}
		else if (IRQNumber >= 64 && IRQNumber < 96)
		{
			//Configure NVIC_ISER2 - 64 to 95
			*NVIC_ISER2 |= (1 << (IRQNumber % 64) );
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//Configure NVIC_ICER0 - 0 to 31
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if (IRQNumber > 31 && IRQNumber < 64)
		{
			//Configure NVIC_ICER1 - 31 to 63
			*NVIC_ICER1 |= (1 << (IRQNumber % 32) );
		}
		else if (IRQNumber >= 64 && IRQNumber < 96)
		{
			//Configure NVIC_ICER2 - 64 to 95
			*NVIC_ICER2 |= (1 << (IRQNumber % 64) );
		}
	}
}


/********************************************************************************************
 * @fn      		  - GPIO_IRQPriorityConfig
 *
 * @brief             - This function handles the IRQ priority
 *
 * @param[in]         - IRQ Number
 * @param[in]		  - IRQ priority
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/

void GPIO_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx ) |= (IRQPriority << shift_amount);
}

/********************************************************************************************
 * @fn      		  - GPIO_IRQHandling
 *
 * @brief             - This function handles the IRQ
 *
 * @param[in]         - GPIO pin Number
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void GPIO_IRQHandling(uint8_t PinNumber)
{
	//Clear the EXTI PR Register corresponding to the Pin Number
	if(EXTI->PR & (1 << PinNumber))
	{
		//clear
		EXTI->PR |= (1 << PinNumber);
	}

}

