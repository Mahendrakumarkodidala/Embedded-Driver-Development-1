/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Feb 5, 2024
 *      Author: mahen
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"

#include <stdint.h>

/*
 * Configuration Structure for SPIx Peripheral
 */
typedef struct
{
	uint8_t SPI_DeviceMode;  				/*!< Possible values from @SPI_DeviceMode >*/
	uint8_t SPI_BusConfig;					/*!< Possible values from @SPI_BusConfig >*/
	uint8_t SPI_SCLKSpeed;					/*!< Possible values from @SPI_SCLKSpeed >*/
	uint8_t SPI_DFF;						/*!< Possible values from @SPI_DFF >*/
	uint8_t SPI_CPOL;						/*!< Possible values from @SPI_CPOL >*/
	uint8_t SPI_CPHA;						/*!< Possible values from @@SPI_CPHA >*/
	uint8_t SPI_SSM;						/*!< Possible values from @SPI_SSM >*/
}SPI_Config_t;


/*
 * Configuration Structure for SPIx Peripheral
 */
typedef struct
{
	SPI_RegDef_t	*pSPIx;			/*!< This holds the base address of the SPIx (x : 1,2,3) peripheral >*/
	SPI_Config_t	SPIConfig;		/*!< This Configurational details of the SPIx (x : 1,2,3) peripheral >*/
	uint8_t			*pTxBuffer;		/*!< To store the application Tx buffer address  >*/
	uint8_t			*pRxBuffer;		/*!< To store the application Rx buffer addressl >*/
	uint32_t		TXLen;			/*!< To store Tx Length >*/
	uint32_t		RXLen;			/*!< To store Rx Length >*/
	uint8_t			TXState;		/*!< To store Tx State, Possible values from @SPI_APP_STATE >*/
	uint8_t			RXState;		/*!< To store Rx state, Possible values from @SPI_APP_STATE >*/
}SPI_Handle_t;


/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER  	1
#define SPI_DEVICE_MODE_SLAVE  		0


/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD						1
#define SPI_BUS_CONFIG_HD						2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY			3


/*
 * @SPI_SCLKSpeed
 */
#define SPI_SCLK_SPEED_DIV2				0
#define SPI_SCLK_SPEED_DIV4				1
#define SPI_SCLK_SPEED_DIV8				2
#define SPI_SCLK_SPEED_DIV16			3
#define SPI_SCLK_SPEED_DIV32			4
#define SPI_SCLK_SPEED_DIV64			5
#define SPI_SCLK_SPEED_DIV128			6
#define SPI_SCLK_SPEED_DIV256			7


/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS		0
#define SPI_DFF_16BITS		1


/*
 * @SPI_CPOL
 */
#define SPI_CPOL_HIGH 	1
#define SPI_CPOL_LOW 	0


/*
 * @SPI_CPHA
 */
#define SPI_CPHA_HIGH 	1
#define SPI_CPHA_LOW 	0


/*
 * @SPI_SSM
 */
#define SPI_SSM_EN 	1
#define SPI_SSM_DI 	0

/*
 * @SPI_APP_STATE
 */
#define SPI_READY			0
#define SPI_BUSY_IN_RX		1
#define SPI_BUSY_IN_TX		2

/*
 * SPI Application States
 */
#define SPI_EVENT_TX_CMPLT   1
#define SPI_EVENT_RX_CMPLT   2
#define SPI_EVENT_OVR_ERR	 3
#define SPI_EVENT_CRC_ERR	 4

/*
 * SPI_SR Flag Definitions
 */
#define SPI_RXNE_FLAG 			(1 << SPI_SR_RXNE)
#define SPI_TXE_FLAG 			(1 << SPI_SR_TXE)
#define SPI_CHSIDE_FLAG 		(1 << SPI_SR_CHSIDE)
#define SPI_UDR_FLAG 			(1 << SPI_SR_UDR)
#define SPI_CRCERR_FLAG 		(1 << SPI_SR_CRCERR)
#define SPI_MODF_FLAG 			(1 << SPI_SR_MODF)
#define SPI_OVR_FLAG 			(1 << SPI_SR_OVR)
#define SPI_BSY_FLAG 			(1 << SPI_SR_BSY)
#define SPI_FRE_FLAG 			(1 << SPI_SR_FRE)

/*********************************************************************************************************
 * 									APIs Supported by this Driver
 * 								For info, check the function definitions
 *********************************************************************************************************/
/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);


/*
 * Init and De-Init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data read and write
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len );
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len );

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig (uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);


/*
 * Other Peripheral Control APIs
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_PeripheralControl ( SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/*
 * Application Callback
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEV);






#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
