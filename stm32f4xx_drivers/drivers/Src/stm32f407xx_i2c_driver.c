/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Feb 11, 2024
 *      Author: mahen
 */
#include "stm32f407xx.h"

#include <stdint.h>

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr );
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr );
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);

/************************************Helper Functions********************************************************/

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= ( 1 << I2C_CR1_START);
}


uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}


static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1);
	pI2Cx->DR |= SlaveAddr;
}


static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= 1;
	pI2Cx->DR |= SlaveAddr;
}


static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
	uint32_t dummyRead;
	//Check for device mode
	if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	{
		//Master Mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize == 1)
			{
				//Disable the ACK
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE	);

				//Clear the ADDR flag
				dummyRead = pI2CHandle->pI2Cx->SR1;
				dummyRead = pI2CHandle->pI2Cx->SR2;
				(void)dummyRead;
			}
		}
		else
		{
			//Clear the ADDR flag
			dummyRead = pI2CHandle->pI2Cx->SR1;
			dummyRead = pI2CHandle->pI2Cx->SR2;
			(void)dummyRead;
		}
	}else
	{
		//Slave Mode
		//Clear the ADDR flag
		dummyRead = pI2CHandle->pI2Cx->SR1;
		dummyRead = pI2CHandle->pI2Cx->SR2;
		(void)dummyRead;

	}
}



void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= ( 1 << I2C_CR1_STOP);
}


void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//enable the ack
		pI2Cx->CR1 |= ( 1 << I2C_CR1_ACK);
	}else
	{
		//disable the ack
		pI2Cx->CR1 &= ~( 1 << I2C_CR1_ACK);
	}
}


static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->TxLen > 0)
	{
		//1. Load the data into DR
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);

		//2. Decrement the TxLen
		pI2CHandle->TxLen--;

		//3. Increment the buffer address
		pI2CHandle->pTxBuffer++;
		}

}


static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
	//data Reception
	if(pI2CHandle->RxSize == 1)
	{
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLen--;
	}

	if(pI2CHandle->RxSize > 1)
	{
		if(pI2CHandle->RxLen == 2)
		{
			//Clear ACK
			I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE	);
		}

		//Read DR
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->pRxBuffer++;
		pI2CHandle->RxLen--;
	}

	if(pI2CHandle->RxLen == 0)
	{
		//Close I2C data reception and notify the application

		//1.Generate the stop condition
		if(pI2CHandle->Sr == I2C_DISABLE_SR)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}

		//2. Reset all the members of the Handle structure
		I2C_CloseReceiveData(pI2CHandle);

		//3. Notify the application
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
	}
}


void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	//Disable ITBUFEN Control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Disable ITEVFEN Control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;

}


void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	//Disable ITBUFEN Control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Disable ITEVFEN Control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
	}
}

void I2c_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}else
	{
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN);
	}
}

/************************************Peripheral Clock setup********************************************************/

/********************************************************************************************
 * @fn      		  - I2C_PeripheralControl
 *
 * @brief             - This function enables or disables peripheral clock for the given I2C port
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]         - ENABLE or DISABLE macros
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			if(pI2Cx == I2C1)
			{
				I2C1_PCLK_EN();
			}

			if(pI2Cx == I2C2)
			{
				I2C2_PCLK_EN();
			}

			if(pI2Cx == I2C3)
			{
				I2C3_PCLK_EN();
			}

		}else
		{
			if(pI2Cx == I2C1)
			{
				I2C1_PCLK_DI();
			}

			if(pI2Cx == I2C2)
			{
				I2C2_PCLK_DI();
			}

			if(pI2Cx == I2C3)
			{
				I2C3_PCLK_DI();
			}

		}
}


/************************************Init and De-Init********************************************************/

/********************************************************************************************
 * @fn      		  - I2C_Init
 *
 * @brief             - This function Initializes the given I2C Peripheral
 *
 * @param[in]         - Pointer to I2C handle structure that contains base address of the I2C
 * 						peripheral and pin Configurations.
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	//Configure the I2C Peripheral Clock
	I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE);

	//CR1 - Enable the ACKing
	uint8_t tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << I2C_CR1_ACK;
	pI2CHandle->pI2Cx->CR1 |= tempreg;


	//CR2 - Configure the FREQ in CR2 value to APB Bus value
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 |= tempreg & 0x3F;

	//OAR1 - Configure the device address
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 |= tempreg;

	//CCR Calculations
	uint16_t ccr_value = 0;
	tempreg = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Standard Mode
		ccr_value = (RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= (ccr_value & 0xFFF);

	}else
	{
		//Fast mode
		tempreg |= (1 << 15);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}else
		{
			ccr_value = (RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}

		tempreg |= (ccr_value & 0xFFF);
	}

	pI2CHandle->pI2Cx->CCR |= tempreg;

	tempreg = 0;

	//TRISE Calculation
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Standard Mode
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;
	}else
	{
		//Fast mode
		tempreg = ( (RCC_GetPCLK1Value() * 300) / 1000000000U) + 1;
	}

	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);


}

/********************************************************************************************
 * @fn      		  - I2C_DeInit
 *
 * @brief             - This function Deinitializes the given I2C Peripheral
 *
 * @param[in]         - base address of the I2C peripheral
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}

	if(pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}

	if(pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}

}



/************************************Data send and receive********************************************************/

/********************************************************************************************
 * @fn      		  - I2C_SendData
 *
 * @brief             - This function sends the data from the given I2Cx peripheral
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]		  - Transfer Buffer pointer
 * @param[in]		  - Length of data
 *
 * @return            -  none
 *
 * @Note              -  This is Blocking Call Function or Polling Call Function

 *********************************************************************************************/
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	//1. Generate START Condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2.Confirm that START generation is completed by checking the SB flag in the SR1
	//(Until SB is cleared SCL will be stretched - LOW)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB) );

	//3. Send the Address of the Slave with R/nW bit set to W (0)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr );

	//4. Confirm that Address phase is completed by checking the ADDR flag in the SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR) );

	//5. Clear the ADDR flag according to its software sequence
	//(Until ADDR is cleared SCL will be stretched - LOW)
	I2C_ClearADDRFlag(pI2CHandle);

	//6. Send data until the length becomes 0.
	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
		pI2CHandle->pI2Cx->DR = *pTxbuffer;
		pTxbuffer++;
		Len--;
	}

	//7. When length becomes zero wait for TXE = 1 and BTF = 1 before generating stop condition.
		//1. TXE = 1 and BTF = 1 means both SR and DR are empty and next transmission should begin.
		//2. When BTF = 1 , SCL will be Stretched.
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//8. Generate STOP condition and master need not to wait for the completion of stop condition.
	//(Generating STOP, automatically clears BTF)
	if(Sr == I2C_DISABLE_SR)
	{
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}

}


/********************************************************************************************
 * @fn      		  - I2C_ReceiveData
 *
 * @brief             - This function reads the data from the given I2C peripheral
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]		  - Receiver Buffer pointer
 * @param[in]		  - Length of data
 *
 * @return            -  none
 *
 * @Note              -  This is Blocking Call Function or Polling Call Function

 *********************************************************************************************/
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB) );

	//3. Send the address of the slave with r/nw bit set to R(1) (total 8 bits )
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr );

	//4. wait until address phase is completed by checking the ADDR flag in teh SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR) );

	//procedure to read only 1 byte from slave
	if(Len == 1)
	{
		//Disable Acking
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE	);

		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//wait until  RXNE becomes 1
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

		//generate STOP condition
		if(Sr == I2C_DISABLE_SR)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}

		//read data in to buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;
	}

    //procedure to read data from slave when Len > 1
	if(Len > 1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//read the data until Len becomes zero
		for ( uint32_t i = Len ; i > 0 ; i--)
		{
			//wait until RXNE becomes 1
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

			if(i == 2) //if last 2 bytes are remaining
			{
				//Disable Acking
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE	);

				//generate STOP condition
				if(Sr == I2C_DISABLE_SR)
				{
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
				}

			}

			//read the data from data register in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

			//increment the buffer address
			pRxBuffer++;
		}
	}
	//re-enable ACKing
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		//Enabling PE
		I2C1 -> CR1 |= ( 1 << I2C_CR1_PE);

		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE );
	}
}


/********************************************************************************************
 * @fn      		  - I2C_MasterSendDataIT
 *
 * @brief             - This function sends the data from the given I2Cx peripheral
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]		  - Transfer Buffer pointer
 * @param[in]		  - Length of data
 *
 * @return            -  none
 *
 * @Note              -  This is Blocking Call Function or Polling Call Function

 *********************************************************************************************/
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxbuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}
	return busystate;
}


/********************************************************************************************
 * @fn      		  - I2C_MasterReceiveDataIT
 *
 * @brief             - This function reads the data from the given I2C peripheral
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]		  - Receiver Buffer pointer
 * @param[in]		  - Length of data
 *
 * @return            -  none
 *
 * @Note              -  This is Blocking Call Function or Polling Call Function

 *********************************************************************************************/
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}
	return busystate;
}

void I2C_SlaveSendData(I2C_RegDef_t *pI2C, uint8_t data)
{
	pI2C->DR = data;
}
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C)
{
	return (uint8_t) pI2C->DR;
}


/************************************IRQ Configuration and ISR handling***************************************************/

/********************************************************************************************
 * @fn      		  - I2C_IRQInterruptConfig
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
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
 * @fn      		  - I2C_IRQPriorityConfig
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
void I2C_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx ) |= (IRQPriority << shift_amount);
}

/********************************************************************************************
 * @fn      		  - I2C_EV_IRQHandling
 *
 * @brief             - This function handles the IRQ
 *
 * @param[in]         - Pointer to I2C handle structure that contains base address of the I2C
 * 						peripheral and pin Configurations.
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	//Interrupt handling for both master and slave mode of a device

	uint32_t temp1,temp2,temp3;

	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);

	//1. Handle For interrupt generated by SB event
	//	Note : SB flag is only applicable in Master mode
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);

	if(temp1 && temp3)
	{
		//SB Event
		//This block is not executed in slave mode because slave SB is always zero.

		//Address phase
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}

	}


	//2. Handle For interrupt generated by ADDR event
	//Note : When master mode : Address is sent
	//		 When Slave mode   : Address matched with own address
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);

	if(temp1 && temp3)
	{
		//ADDR Event
		I2C_ClearADDRFlag(pI2CHandle);
	}


	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);

	if(temp1 && temp3)
	{
		//BTF Event
		//if BTF = 1 in transmission, both SR and DR are empty.
		//if BTF = 1 in reception, both SR and DR are full.
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			//TXE is set
			if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE))
			{
				if(pI2CHandle->TxLen == 0)
				{
					//BTF and TXE are set
					//1. Generate the STOP Condition
					if(pI2CHandle->Sr == I2C_DISABLE_SR)
					{
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}

					//2. Reset all the members of the Handle structure
					I2C_CloseSendData(pI2CHandle);

					//3. Notify the application
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				}
			}
		}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			;
		}
	}


	//4. Handle For interrupt generated by STOPF event
	// Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);

	if(temp1 && temp3)
	{
		//STOPF is Set and It should be Cleared
		//STOPF is Cleared by reading SR1 and writing to CR2
		pI2CHandle->pI2Cx->CR1 |= 0x0000;

		//Notify the application that STOP is detected
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
	}


	//5. Handle For interrupt generated by TXE event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE);

	if(temp1 && temp2 && temp3)
	{
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			//TXE Event -> DR is empty, so do data transmission
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			{
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}else
		{
			//Slave Mode
			if(!(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
			{
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
			}
		}
	}



	//6. Handle For interrupt generated by RXNE event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE);

	if(temp1 && temp2 && temp3)
	{
		//Device Mode
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			//RXNE Event
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			{
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}else
		{
			//Slave Mode
			if(!(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
			{
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
			}
		}
	}
}

/********************************************************************************************
 * @fn      		  - I2C_ER_IRQHandling
 *
 * @brief             - This function handles the IRQ
 *
 * @param[in]         - Pointer to I2C handle structure that contains base address of the I2C
 * 						peripheral and pin Configurations.
 *
 * @return            -  none
 *
 * @Note              -  none

 *********************************************************************************************/
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);
	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);
	}

}
