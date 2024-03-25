/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Feb 5, 2024
 *      Author: mahen
 */
#include "stm32f407xx.h"

#include <stdint.h>

static void SPI_Txe_Interrupt_Handle(SPI_Handle_t *pSPIHandle);
static void SPI_Rxne_Interrupt_Handle(SPI_Handle_t *pSPIHandle);
static void SPI_Ovr_Interrupt_Handle(SPI_Handle_t *pSPIHandle);


/************************************Peripheral Clock setup********************************************************/

/********************************************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]         - ENABLE or DISABLE macros
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_EN();
			}

			if(pSPIx == SPI2)
			{
				SPI2_PCLK_EN();
			}

			if(pSPIx == SPI3)
			{
				SPI3_PCLK_EN();
			}

			if(pSPIx == SPI4)
			{
				SPI4_PCLK_EN();
			}

		}else
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_DI();
			}

			if(pSPIx == SPI2)
			{
				SPI2_PCLK_DI();
			}

			if(pSPIx == SPI3)
			{
					SPI3_PCLK_DI();
			}

			if(pSPIx == SPI4)
			{
				SPI4_PCLK_DI();
			}
		}
}


/************************************Init and De-Init********************************************************/

/********************************************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             - This function Initializes the given SPI Peripheral
 *
 * @param[in]         - Pointer to SPI handle structure that contains base address of the SPI
 * 						peripheral and pin Configurations.
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//Configure the SPI Peripheral Clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//Configure the SPI_CR1 Register using Temp Register
		uint32_t tempreg = 0;

	//1. Configure the SPI_DeviceMode
		tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR ;

	//2. Configure the SPI_BusConfig
		if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
		{
			//1. Full-Duplex -> BIDIMODE bit should be cleared
			tempreg &= ~(1 << SPI_CR1_BIDIMODE);

		}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
		{
			//2. Half-Duplex -> BIDIMODE bit should be Set
			tempreg |= (1 << SPI_CR1_BIDIMODE);

		}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
		{
			//3. Simplex -> BIDIMODE bit should be Cleared & RXONLY bit should be SET
			tempreg &= ~(1 << SPI_CR1_BIDIMODE);
			tempreg |= (1 << SPI_CR1_RXONLY);
		}

	//3. Configure the SPI_SCLKSpeed BR[x]
		tempreg |= pSPIHandle->SPIConfig.SPI_SCLKSpeed << SPI_CR1_BR ;

	//4. Configure the SPI_DFF
		tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF ;

	//5. Configure the SPI_CPOL
		tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL ;

	//6. Configure the SPI_CPHA
		tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA ;

	//7. Configure the SPI_SSM
		tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM ;


		pSPIHandle->pSPIx->CR1 = tempreg;

}

/********************************************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             - This function Deinitializes the given SPI Peripheral
 *
 * @param[in]         - base address of the SPI peripheral
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}

	if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}

	if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}

	if(pSPIx == SPI4)
	{
		SPI4_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}
/************************************Data read and write********************************************************/

/********************************************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             - This function sends the data from the given SPIx peripheral
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]		  - Transfer Buffer pointer
 * @param[in]		  - Length of data
 *
 * @return            -  none
 *
 * @Note              -  This is Blocking Call Function or Polling Call Function

 *********************************************************************************************/
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait until the Tx buffer is Empty
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG)  == FLAG_RESET );

		//2. Checking DFF In CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			//16-bit
			pSPIx->DR = *(uint16_t *)pTxBuffer;
			Len--;
			Len--;
			(uint16_t *)pTxBuffer++;
		}else
		{
			//8-bit
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}

/********************************************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             - This function reads the data from the given SPI peripheral
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]		  - Receiver Buffer pointer
 * @param[in]		  - Length of data
 *
 * @return            -  none
 *
 * @Note              -  This is Blocking Call Function or Polling Call Function

 *********************************************************************************************/
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len )
{
	while(Len>0)
	{
		//1. Wait until the Rx buffer is Full
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		//2. Checking DFF In CR1
		if(pSPIx ->CR1 & (1 << SPI_CR1_DFF))
		{
			//16-bits
			*((uint16_t *)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t *)pRxBuffer++;
		}
		else
		{
			//8-bits
			*(pRxBuffer) = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}
	}
}


/************************************IRQ Configuration and ISR handling***************************************************/

/********************************************************************************************
 * @fn      		  - SPI_IRQInterruptConfig
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
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
 * @fn      		  - SPI_IRQPriorityConfig
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
void SPI_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx ) |= (IRQPriority << shift_amount);
}

/********************************************************************************************
 * @fn      		  - SPI_IRQHandling
 *
 * @brief             - This function handles the IRQ
 *
 * @param[in]         - Pointer to SPI handle structure that contains base address of the SPI
 * 						peripheral and pin Configurations.
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	//Checking TXE Flag
	uint8_t temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	uint8_t temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		//Handling TXE Error
		SPI_Txe_Interrupt_Handle(pSPIHandle);
	}

	//Checking RXNE Flag
	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2)
	{
		//Handling RXNE Error
		SPI_Rxne_Interrupt_Handle(pSPIHandle);
	}

	//Checking OVR Flag
	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2)
	{
		//Handling OVR Error
		SPI_Ovr_Interrupt_Handle(pSPIHandle);
	}

}


/********************************************************************************************
 * @fn      		  - SPI_SendDataIT
 *
 * @brief             - This function sends the data from the given SPIx peripheral
 *
 * @param[in]         - Pointer to SPI handle structure that contains base address of the SPI
 * 						peripheral and pin Configurations.
 * @param[in]		  - Transfer Buffer pointer
 * @param[in]		  - Length of data
 *
 * @return            -  none
 *
 * @Note              -  This is Non - Blocking Call Function or Interrupt Call Function

 *********************************************************************************************/
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TXState;

	if(pSPIHandle->TXState != SPI_BUSY_IN_TX)
	{
		//1. Save the Tx buffer address and len information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TXLen = Len;

		//2. Mark the SPI state as busy in transmission
		pSPIHandle->TXState = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

		//4. Data Transmission will be handled by the ISR Code
	}

	return state;

}


/********************************************************************************************
 * @fn      		  - SPI_ReceiveDataIT
 *
 * @brief             - This function reads the data from the given SPI peripheral
 *
 * @param[in]         - Pointer to SPI handle structure that contains base address of the SPI
 * 						peripheral and pin Configurations.
 * @param[in]		  - Receiver Buffer pointer
 * @param[in]		  - Length of data
 *
 * @return            -  none
 *
 * @Note              -  This is Non - Blocking Call Function or Interrupt Call Function

 *********************************************************************************************/
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len )
{
	uint8_t state = pSPIHandle->RXState;

	if(pSPIHandle->RXState != SPI_BUSY_IN_RX)
	{
		//1. Save the Tx buffer address and len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RXLen = Len;

		//2. Mark the SPI state as busy in transmission
		pSPIHandle->RXState = SPI_BUSY_IN_RX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

		//4. Data Transmission will be handled by the ISR Code
	}

	return state;
}

/************************************Other Peripheral Control APIs***************************************************/

/********************************************************************************************
 * @fn      		  - SPI_PeripheralControl
 *
 * @brief             - This function enables the Peripheral
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]         - ENABLE or DISABLE
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void SPI_PeripheralControl ( SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}


/********************************************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 * @brief             - This function enables the SSI when SSM is enabled
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]         - ENABLE or DISABLE
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
		{
			pSPIx->CR1 |= (1 << SPI_CR1_SSI);
		}
		else
		{
			pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
		}
}

/********************************************************************************************
 * @fn      		  - SPI_SSOEConfig
 *
 * @brief             - This function enables the SSOE
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]         - ENABLE or DISABLE
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
		{
			pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
		}
		else
		{
			pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
		}
}


/************************************Helper Function Implementation***************************************************/
static void SPI_Txe_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	//Checking DFF In CR1
	if( pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		//16-bit
		pSPIHandle->pSPIx->DR = *(uint16_t *)pSPIHandle->pTxBuffer;
		pSPIHandle->TXLen--;
		pSPIHandle->TXLen--;
		(uint16_t *)pSPIHandle->pTxBuffer++;
	}else
	{
		//8-bit
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TXLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(	! pSPIHandle->TXLen)
	{
		//TxLen is Zero, Close the SPI transmission, inform the application that Tx is over

		//Preventing the interrupt
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);


	}
}


static void SPI_Rxne_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	//Checking DFF In CR1
	if( pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		//16-bit
		*((uint16_t *)pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RXLen--;
		pSPIHandle->RXLen--;
		pSPIHandle->pRxBuffer++;
		pSPIHandle->pRxBuffer++;
	}else
	{
		//8-bit
		*pSPIHandle->pRxBuffer = (uint8_t) pSPIHandle->pSPIx->DR ;
		pSPIHandle->RXLen--;
		pSPIHandle->pRxBuffer++;
	}

	if(	! pSPIHandle->RXLen)
	{
		//TxLen is Zero, Close the SPI transmission, inform the application that Tx is over

		//Preventing the interrupt
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);


	}
}


static void SPI_Ovr_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;

	//1. Clear the OVR Flag
	if(pSPIHandle->TXState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;

	}

	(void)temp;

	//2.Inform Application
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);


}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TXLen = 0;
	pSPIHandle->TXState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RXLen = 0;
	pSPIHandle->RXState = SPI_READY;
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;

	temp = pSPIx->DR;
	temp = pSPIx->SR;

	(void)temp;
}


__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEV)
{
	//This is a weak implementation. The user application may override this function

}
