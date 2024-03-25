/*
 * stm32f407xx.h
 *
 *  Created on: Jan 31, 2024
 *      Author: mahen
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#include <stddef.h>

#define __vo		volatile
#define __weak 		__attribute__((weak))

/*****************************************Processor Specific Details**************************************
 * Addresses of ARM Coretx Mx - NVIC ISERx Registers
 */
#define NVIC_ISER0		( (__vo uint32_t*)0xE000E100 )
#define NVIC_ISER1		( (__vo uint32_t*)0xE000E104 )
#define NVIC_ISER2		( (__vo uint32_t*)0xE000E108 )
#define NVIC_ISER3		( (__vo uint32_t*)0xE000E10C )
#define NVIC_ISER4		( (__vo uint32_t*)0xE000E110 )
#define NVIC_ISER5		( (__vo uint32_t*)0xE000E114 )
#define NVIC_ISER6		( (__vo uint32_t*)0xE000E118 )
#define NVIC_ISER7		( (__vo uint32_t*)0xE000E11C )


/*
 * Addresses of ARM Coretx Mx - NVIC ICERx Registers
 */
#define NVIC_ICER0		( (__vo uint32_t*)0XE000E180 )
#define NVIC_ICER1		( (__vo uint32_t*)0XE000E184 )
#define NVIC_ICER2		( (__vo uint32_t*)0XE000E188 )
#define NVIC_ICER3		( (__vo uint32_t*)0XE000E18C )
#define NVIC_ICER4		( (__vo uint32_t*)0XE000E190 )
#define NVIC_ICER5		( (__vo uint32_t*)0XE000E194 )
#define NVIC_ICER6		( (__vo uint32_t*)0XE000E198 )
#define NVIC_ICER7		( (__vo uint32_t*)0XE000E19C )


/*
 * Addresses of ARM Coretx Mx - NVIC IRPx Registers
 */
#define NVIC_PR_BASE_ADDR		( (__vo uint32_t*)0xE000E400 )


/*
 * ARM Coretx Mx - Number of priority bits implemented
 */
#define NO_PR_BITS_IMPLEMENTED			4



/*****************************************MCU Specific Details**************************************
 * Base addresses of Flash and SRAM Memories
 */
#define FLASH_BASEADDR				0x08000000U 	/*FLash Base Address */
#define SRAM1_BASEADDR				0x20000000U 	/*SRAM1 Base Address (112 kb)*/
#define SRAM2_BASEADDR				0x2001C000U		/*SRAM2 Base Address (SRAM1 + 112 kb)*/
#define ROM							0x1FFF0000U		/*ROM Base Address */
#define SRAM 						SRAM1_BASEADDR


/*
 * Base addresses of different bus domains AHBx and APBx
 */
#define PERIPH_BASE					0X40000000U
#define APB1PERIPH_BASE				PERIPH_BASE
#define APB2PERIPH_BASE				0X40010000U
#define AHB1PERIPH_BASE				0X40020000U
#define AHB2PERIPH_BASE				0X50000000U



/*
 * Base addresses of different Peripherals on AHB1
 */
#define GPIOA_BASEADDR				(AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR				(AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR				(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR				(AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR				(AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR				(AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR				(AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR				(AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASEADDR				(AHB1PERIPH_BASE + 0x2000)
#define GPIOJ_BASEADDR				(AHB1PERIPH_BASE + 0x2400)
#define GPIOK_BASEADDR				(AHB1PERIPH_BASE + 0x2800)
#define RCC_BASEADDR				(AHB1PERIPH_BASE + 0x3800)


/*
 * Base addresses of different Peripherals on APB1
 */
#define I2C1_BASEADDR				(APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR				(APB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDR				(APB1PERIPH_BASE + 0x5C00)

#define SPI2_BASEADDR				(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR				(APB1PERIPH_BASE + 0x3C00)

#define USART2_BASEADDR				(APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR				(APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR				(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR				(APB1PERIPH_BASE + 0x5000)



/*
 * Base addresses of different Peripherals on APB2
 */
#define EXTI_BASEADDR				(APB2PERIPH_BASE + 0x3C00)

#define SPI1_BASEADDR				(APB2PERIPH_BASE + 0x3000)
#define SPI4_BASEADDR				(APB2PERIPH_BASE + 0x3400)

#define SYSCFG_BASEADDR				(APB2PERIPH_BASE + 0x3800)

#define USART1_BASEADDR				(APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDR				(APB2PERIPH_BASE + 0x1400)




/**************************** Peripheral Register Definition Structures ********************************/

/*
 * Note : Registers of a peripheral are specific to MCU
 * Please check your device RM.
 */

/*
 * GPIO Peripheral Register Definition Structure
 */
typedef struct
{
	__vo uint32_t MODER;		/*!< GPIO port mode register,							Address offset: 0x00 >*/
	__vo uint32_t OTYPER;		/*< GPIO port output type register,						Address offset: 0x04 >*/
	__vo uint32_t OSPEEDR;		/*< GPIO port output speed register,					Address offset: 0x08 >*/
	__vo uint32_t PUPDR;		/*< GPIO port pull-up/pull-down register,				Address offset: 0x0C >*/
	__vo uint32_t IDR;			/*< GPIO port input data register,						Address offset: 0x10 >*/
	__vo uint32_t ODR;			/*< GPIO port output data register,						Address offset: 0x14 >*/
	__vo uint32_t BSRR;			/*< GPIO port bit set/reset register,					Address offset: 0x18 >*/
	__vo uint32_t LCKR;			/*< GPIO port configuration lock register,				Address offset: 0x1C >*/
	__vo uint32_t AFR[2];		/*< AFR[0] : GPIO alternate function low register,		Address offset: 0x20 >*/
								/*< AFR[1] : GPIO alternate function high register,		Address offset: 0x24 >*/
}GPIO_RegDef_t;



/*
 * RCC Peripheral Register Definition Structure
 */
typedef struct
{
	__vo uint32_t CR;			/*< RCC clock control register ,									Address offset: 0x00 >*/
	__vo uint32_t PLLCFGR;		/*< RCC PLL configuration register ,								Address offset: 0x04>*/
	__vo uint32_t CFGR;			/*< RCC clock configuration register ,								Address offset: 0x08>*/
	__vo uint32_t CIR;			/*< RCC clock interrupt register ,									Address offset: 0x0C>*/
	__vo uint32_t AHB1RSTR;		/*< RCC AHB1 peripheral reset register ,							Address offset: 0x10>*/
	__vo uint32_t AHB2RSTR;		/*< RCC AHB2 peripheral reset register ,							Address offset: 0x14>*/
	__vo uint32_t AHB3RSTR;		/*< RCC AHB3 peripheral reset register ,							Address offset: 0x18 >*/
	uint32_t RESERVED0;			/*< RESERVED ,														Address offset: 0x1C >*/
	__vo uint32_t APB1RSTR;		/*< RCC APB1 peripheral reset register ,							Address offset: 0x20 >*/
	__vo uint32_t APB2RSTR;		/*< RCC APB2 peripheral reset register ,							Address offset: 0x24 >*/
	uint32_t RESERVED1[2];		/*< RESERVED ,												Address offset: 0x28, 0x2C >*/
	__vo uint32_t AHB1ENR;		/*< RCC AHB1 peripheral clock register ,							Address offset: 0x30 >*/
	__vo uint32_t AHB2ENR;		/*< RCC AHB2 peripheral clock enable register ,						Address offset: 0x34 >*/
	__vo uint32_t AHB3ENR;		/*< RCC AHB3 peripheral clock enable register ,						Address offset: 0x38 >*/
	uint32_t RESERVED2;		/*< RESERVED ,														Address offset: 0x3C >*/
	__vo uint32_t APB1ENR;		/*< RCC APB1 peripheral clock enable register ,						Address offset: 0x40 >*/
	__vo uint32_t APB2ENR;		/*< RCC APB2 peripheral clock enable register ,						Address offset: 0x44 >*/
	uint32_t RESERVED3[2];		/*< RESERVED ,												Address offset: 0x48, 0x4C >*/
	__vo uint32_t AHB1LPENR;	/*< RCC AHB1 peripheral clock enable in low power mode register ,	Address offset: 0x50 >*/
	__vo uint32_t AHB2LPENR;	/*< RCC AHB2 peripheral clock enable in low power mode register ,	Address offset: 0x54 >*/
	__vo uint32_t AHB3LPENR;	/*< RCC AHB3 peripheral clock enable in low power mode register ,	Address offset: 0x58 >*/
	uint32_t RESERVED4;			/*< RESERVED ,														Address offset: 0x5C >*/
	__vo uint32_t APB1LPENR;	/*< RCC APB1 peripheral clock enable in low power mode register ,	Address offset: 0x60 >*/
	__vo uint32_t APB2LPENR;	/*< RCC APB2 peripheral clock enabled in low power mode register ,	Address offset: 0x64 >*/
	uint32_t RESERVED5[2];		/*< RESERVED ,												Address offset: 0x68, 0x6C >*/
	__vo uint32_t BDCR;			/*< RCC Backup domain control register  ,							Address offset: 0x70 >*/
	__vo uint32_t CSR;			/*< RCC clock control & status register ,							Address offset: 0x74 >*/
	uint32_t RESERVED6[2];		/*< RESERVED ,												Address offset: 0x78, 0x7C >*/
	__vo uint32_t SSCGR;		/*< RCC spread spectrum clock generation register ,					Address offset: 0x80 >*/
	__vo uint32_t PLLI2SCFGR;	/*< RCC PLLI2S configuration register ,								Address offset: 0x84 >*/
	__vo uint32_t PLLSAICFGR;	/*< RCC PLL configuration register ,								Address offset: 0x88 >*/
	__vo uint32_t DCKCFGR;		/*< RCC Dedicated Clock Configuration Register ,					Address offset: 0x8C >*/
}RCC_RegDef_t;


/*
 * EXTI Peripheral Register Definition Structure
 */
typedef struct
{
	__vo uint32_t IMR;    /*!< Interrupt mask register ,          	  	    Address offset: 0x00 */
	__vo uint32_t EMR;    /*!< Event mask register ,                		Address offset: 0x04 */
	__vo uint32_t RTSR;   /*!< Rising trigger selection register ,       	Address offset: 0x08 */
	__vo uint32_t FTSR;   /*!< Falling trigger selection register , 		Address offset: 0x0C */
	__vo uint32_t SWIER;  /*!< Software interrupt event register ,  		Address offset: 0x10 */
	__vo uint32_t PR;     /*!< Pending register ,                   		Address offset: 0x14 */
}EXTI_RegDef_t;


/*
 * SYSCFG Peripheral Register Definition Structure
 */
typedef struct
{
	__vo uint32_t MEMRMP;    	/*!< SYSCFG memory remap register ,          			  Address offset: 0x00 */
	__vo uint32_t PMC;    		/*!< SYSCFG peripheral mode configuration register ,      Address offset: 0x04 */
	__vo uint32_t EXTICR[4];   	/*!< SYSCFG external interrupt configuration register 1 , Address offset: 0x08 - 0x14 */
	uint32_t	RESERVED1[2];	/*!< Reservation									 , 	  Address offset: 0x18 - 0x1C */
	__vo uint32_t CMPCR;     	/*!< Compensation cell control register ,                 Address offset: 0x20 */
	uint32_t	RESERVED2[2];	/*!< Reservation									 , 	  Address offset: 0x24 - 0x28 */
	__vo uint32_t CFGR;     	/*!< Compensation cell control register ,                 Address offset: 0x2C */
}SYSCFG_RegDef_t;


/*
 * SPI Peripheral Register Definition Structure
 */
typedef struct
{
	__vo uint32_t CR1; 		/*!< SPI control register 1 ,      				Address offset: 0x00 */
	__vo uint32_t CR2; 		/*!< SPI control register 2 ,      				Address offset: 0x04 */
	__vo uint32_t SR;		/*!< SPI status register ,          			Address offset: 0x08 */
	__vo uint32_t DR;		/*!< SPI data register ,          			  	Address offset: 0x0C */
	__vo uint32_t CRCPR;	/*!< SPI CRC polynomial register ,          	Address offset: 0x10 */
	__vo uint32_t RXCRCR; 	/*!< SPI RX CRC register ,          			Address offset: 0x14 */
	__vo uint32_t TXCRCR; 	/*!< SPI TX CRC register ,          			Address offset: 0x18 */
	__vo uint32_t I2SCFGR; 	/*!< SPI_I2S configuration register ,          	Address offset: 0x1C */
	__vo uint32_t I2SPR;	/*!< SPI_I2S prescaler register ,          		Address offset: 0x20 */
}SPI_RegDef_t;


/*
 * I2C Peripheral Register Definition Structure
 */
typedef struct
{
	__vo uint32_t CR1;		/*!< I2C Control register 1 ,      				Address offset: 0x00 */
	__vo uint32_t CR2;		/*!< I2C Control register 2 ,      				Address offset: 0x04 */
	__vo uint32_t OAR1;		/*!< I2C Own address register 1  ,      		Address offset: 0x08 */
	__vo uint32_t OAR2;		/*!< I2C Own address register 2 ,      			Address offset: 0x0C */
	__vo uint32_t DR;		/*!< I2C Data register ,      					Address offset: 0x10 */
	__vo uint32_t SR1;		/*!< I2C Status register 1 ,      				Address offset: 0x14 */
	__vo uint32_t SR2;		/*!< I2C Status register 2 ,      				Address offset: 0x18 */
	__vo uint32_t CCR;		/*!< I2C Clock control register ,      			Address offset: 0x1C */
	__vo uint32_t TRISE;	/*!< I2C TRISE register ,      					Address offset: 0x20 */
	__vo uint32_t FLTR;		/*!< I2C FLTR register ,      					Address offset: 0x24 */
}I2C_RegDef_t;



/*
 * USART Peripheral Register Definition Structure
 */
typedef struct
{
	__vo uint32_t SR;         /*!< USART Status register   								Address offset: 0x00 */
	__vo uint32_t DR;         /*!< USART Data register   								Address offset: 0x04 */
	__vo uint32_t BRR;        /*!< USART Baud rate register    							Address offset: 0x08 */
	__vo uint32_t CR1;        /*!< USART Control register 1    							Address offset: 0x0C */
	__vo uint32_t CR2;        /*!< USART Control register 2    							Address offset: 0x10 */
	__vo uint32_t CR3;        /*!< USART Control register 3    							Address offset: 0x14 */
	__vo uint32_t GTPR;       /*!< USART Guard time and prescaler register   			Address offset: 0x18 */
} USART_RegDef_t;

/*
 * Peripheral Definitions (Peripheral Base addresses typecasted to xxx_RegDef_t)
 */
#define GPIOA 	( (GPIO_RegDef_t*) GPIOA_BASEADDR )
#define GPIOB 	( (GPIO_RegDef_t*) GPIOB_BASEADDR )
#define GPIOC 	( (GPIO_RegDef_t*) GPIOC_BASEADDR )
#define GPIOD 	( (GPIO_RegDef_t*) GPIOD_BASEADDR )
#define GPIOE 	( (GPIO_RegDef_t*) GPIOE_BASEADDR )
#define GPIOF 	( (GPIO_RegDef_t*) GPIOF_BASEADDR )
#define GPIOG 	( (GPIO_RegDef_t*) GPIOG_BASEADDR )
#define GPIOH 	( (GPIO_RegDef_t*) GPIOH_BASEADDR )
#define GPIOI 	( (GPIO_RegDef_t*) GPIOI_BASEADDR )
#define GPIOJ 	( (GPIO_RegDef_t*) GPIOJ_BASEADDR )
#define GPIOK 	( (GPIO_RegDef_t*) GPIOK_BASEADDR )

#define RCC 	( (RCC_RegDef_t*) RCC_BASEADDR )

#define EXTI	( (EXTI_RegDef_t*) EXTI_BASEADDR )

#define SYSCFG	( (SYSCFG_RegDef_t*) SYSCFG_BASEADDR )

#define SPI1    ( (SPI_RegDef_t*) SPI1_BASEADDR )
#define SPI2    ( (SPI_RegDef_t*) SPI2_BASEADDR )
#define SPI3    ( (SPI_RegDef_t*) SPI3_BASEADDR )
#define SPI4    ( (SPI_RegDef_t*) SPI4_BASEADDR )

#define I2C1    ( (I2C_RegDef_t*) I2C1_BASEADDR )
#define I2C2    ( (I2C_RegDef_t*) I2C2_BASEADDR )
#define I2C3    ( (I2C_RegDef_t*) I2C3_BASEADDR )

#define USART1  ( (USART_RegDef_t*)USART1_BASEADDR )
#define USART2  ( (USART_RegDef_t*)USART2_BASEADDR )
#define USART3  ( (USART_RegDef_t*)USART3_BASEADDR )
#define UART4  	( (USART_RegDef_t*)UART4_BASEADDR )
#define UART5  	( (USART_RegDef_t*)UART5_BASEADDR )
#define USART6  ( (USART_RegDef_t*)USART6_BASEADDR )
/*
 * Clock Enable Macros for GPIOx Peripherals
 */
#define GPIOA_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 0) )
#define GPIOB_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 1) )
#define GPIOC_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 2) )
#define GPIOD_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 3) )
#define GPIOE_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 4) )
#define GPIOF_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 5) )
#define GPIOG_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 6) )
#define GPIOH_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 7) )
#define GPIOI_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 8) )
#define GPIOJ_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 9) )
#define GPIOK_PCLK_EN()			( RCC -> AHB1ENR |= (1 << 10) )


/*
 * Clock Enable Macros for I2Cx Peripherals
 */
#define I2C1_PCLK_EN()			( RCC -> APB1ENR |= (1 << 21) )
#define I2C2_PCLK_EN()			( RCC -> APB1ENR |= (1 << 22) )
#define I2C3_PCLK_EN()			( RCC -> APB1ENR |= (1 << 23) )


/*
 * Clock Enable Macros for SPIx Peripherals
 */
#define SPI1_PCLK_EN()			( RCC -> APB2ENR |= (1 << 12) )
#define SPI2_PCLK_EN()			( RCC -> APB1ENR |= (1 << 14) )
#define SPI3_PCLK_EN()			( RCC -> APB1ENR |= (1 << 15) )
#define SPI4_PCLK_EN()			( RCC -> APB2ENR |= (1 << 13) )


/*
 * Clock Enable Macros for USARTx Peripherals
 */
#define USART1_PCLK_EN()			( RCC -> APB2ENR |= (1 << 4) )
#define USART2_PCLK_EN()			( RCC -> APB1ENR |= (1 << 17) )
#define USART3_PCLK_EN()			( RCC -> APB1ENR |= (1 << 18) )
#define UART4_PCLK_EN()				( RCC -> APB1ENR |= (1 << 19) )
#define UART5_PCLK_EN()				( RCC -> APB1ENR |= (1 << 20) )
#define USART6_PCLK_EN()			( RCC -> APB2ENR |= (1 << 5) )


/*
 * Clock Enable Macros for SYSCFG Peripherals
 */
#define SYSCFG_PCLK_EN()			( RCC -> APB2ENR |= (1 << 14) )


/*
 * Clock Disable Macros for GPIOx Peripherals
 */
#define GPIOA_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 0) )
#define GPIOB_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 1) )
#define GPIOC_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 2) )
#define GPIOD_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 3) )
#define GPIOE_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 4) )
#define GPIOF_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 5) )
#define GPIOG_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 6) )
#define GPIOH_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 7) )
#define GPIOI_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 8) )
#define GPIOJ_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 9) )
#define GPIOK_PCLK_DI()			( RCC -> AHB1ENR &= ~(1 << 10) )


/*
 * Clock Disable Macros for I2Cx Peripherals
 */
#define I2C1_PCLK_DI()			( RCC -> APB1ENR &= ~(1 << 21) )
#define I2C2_PCLK_DI()			( RCC -> APB1ENR &= ~(1 << 22) )
#define I2C3_PCLK_DI()			( RCC -> APB1ENR &= ~(1 << 23) )


/*
 * Clock Disable Macros for SPIx Peripherals
 */
#define SPI1_PCLK_DI()			( RCC -> APB2ENR &= ~(1 << 12) )
#define SPI2_PCLK_DI()			( RCC -> APB1ENR &= ~(1 << 14) )
#define SPI3_PCLK_DI()			( RCC -> APB1ENR &= ~(1 << 15) )
#define SPI4_PCLK_DI()			( RCC -> APB2ENR &= ~(1 << 13) )


/*
 * Clock Disable Macros for USARTx Peripherals
 */
#define USART1_PCLK_DI()			( RCC -> APB2ENR &= ~(1 << 4) )
#define USART2_PCLK_DI()			( RCC -> APB1ENR &= ~(1 << 17) )
#define USART3_PCLK_DI()			( RCC -> APB1ENR &= ~(1 << 18) )
#define UART4_PCLK_DI()				( RCC -> APB1ENR &= ~(1 << 19) )
#define UART5_PCLK_DI()				( RCC -> APB1ENR &= ~(1 << 20) )
#define USART6_PCLK_DI()			( RCC -> APB2ENR &= ~(1 << 5) )


/*
 * Clock Disable Macros for SYSCFG Peripherals
 */
#define SYSCFG_PCLK_DI()			( RCC -> APB2ENR &= ~(1 << 14) )


/*
 *Macros to RESET GPIO Peripherals
 */
#define GPIOA_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 0)); (RCC -> AHB1RSTR &= ~(1 << 0));}while(0)
#define GPIOB_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 1)); (RCC -> AHB1RSTR &= ~(1 << 1));}while(0)
#define GPIOC_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 2)); (RCC -> AHB1RSTR &= ~(1 << 2));}while(0)
#define GPIOD_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 3)); (RCC -> AHB1RSTR &= ~(1 << 3));}while(0)
#define GPIOE_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 4)); (RCC -> AHB1RSTR &= ~(1 << 4));}while(0)
#define GPIOF_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 5)); (RCC -> AHB1RSTR &= ~(1 << 5));}while(0)
#define GPIOG_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 6)); (RCC -> AHB1RSTR &= ~(1 << 6));}while(0)
#define GPIOH_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 7)); (RCC -> AHB1RSTR &= ~(1 << 7));}while(0)
#define GPIOI_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 8)); (RCC -> AHB1RSTR &= ~(1 << 8));}while(0)
#define GPIOJ_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 9)); (RCC -> AHB1RSTR &= ~(1 << 9));}while(0)
#define GPIOK_REG_RESET() 		do{(RCC -> AHB1RSTR |= (1 << 10)); (RCC -> AHB1RSTR &= ~(1 << 10));}while(0)


/*
 *Macros to RESET SPI Peripherals
 */
#define SPI1_REG_RESET() 		do{(RCC -> APB2RSTR |= (1 << 12)); (RCC -> APB2RSTR &= ~(1 << 12));}while(0)
#define SPI2_REG_RESET() 		do{(RCC -> APB1RSTR |= (1 << 14)); (RCC -> APB1RSTR &= ~(1 << 14));}while(0)
#define SPI3_REG_RESET() 		do{(RCC -> APB1RSTR |= (1 << 15)); (RCC -> APB1RSTR &= ~(1 << 15));}while(0)
#define SPI4_REG_RESET() 		do{(RCC -> APB2RSTR |= (1 << 13)); (RCC -> APB2RSTR &= ~(1 << 13));}while(0)


/*
 *Macros to RESET SPI Peripherals
 */
#define I2C1_REG_RESET() 		do{(RCC -> APB1RSTR |= (1 << 21)); (RCC -> APB1RSTR &= ~(1 << 21));}while(0)
#define I2C2_REG_RESET() 		do{(RCC -> APB1RSTR |= (1 << 22)); (RCC -> APB1RSTR &= ~(1 << 22));}while(0)
#define I2C3_REG_RESET() 		do{(RCC -> APB1RSTR |= (1 << 23)); (RCC -> APB1RSTR &= ~(1 << 23));}while(0)

/*
 * This macro returns a code( between 0 to 7) for a given GPIO base address(x)
 */
#define GPIO_BASEADDR_TO_CODE(x)	( (x == GPIOA)?0:\
									  (x == GPIOB)?1:\
									  (x == GPIOC)?2:\
									  (x == GPIOD)?3:\
									  (x == GPIOE)?4:\
									  (x == GPIOF)?5:\
									  (x == GPIOG)?6:\
									  (x == GPIOH)?7:\
									  (x == GPIOI)?8:\
									  (x == GPIOJ)?9:0 )

/*
 * EXTI IRQ Numbers
 */
#define IRQ_NO_EXTI0  		6
#define IRQ_NO_EXTI1  		7
#define IRQ_NO_EXTI2  		8
#define IRQ_NO_EXTI3  		9
#define IRQ_NO_EXTI4  		10
#define IRQ_NO_EXTI9_5  	23
#define IRQ_NO_EXTI15_10  	40


/*
 * SPI IRQ Numbers
 */
#define IRQ_NO_SPI1  		35
#define IRQ_NO_SPI2  		36
#define IRQ_NO_SPI3  		51


/*
 * I2C IRQ Numbers
 */
#define IRQ_NO_I2C1_EV  		31
#define IRQ_NO_I2C1_ER  		32
#define IRQ_NO_I2C2_EV  		33
#define IRQ_NO_I2C2_ER  		34
#define IRQ_NO_I2C3_EV   		79
#define IRQ_NO_I2C3_ER   		80


/*
 * USART IRQ Numbers
 */
#define IRQ_NO_USART1	    37
#define IRQ_NO_USART2	    38
#define IRQ_NO_USART3	    39
#define IRQ_NO_UART4	    52
#define IRQ_NO_UART5	    53
#define IRQ_NO_USART6	    71

/*
 * IRQ Priority
 */
#define NVIC_IRQ_PRI0	0
#define NVIC_IRQ_PRI1	1
#define NVIC_IRQ_PRI2	2
#define NVIC_IRQ_PRI3	3
#define NVIC_IRQ_PRI4	4
#define NVIC_IRQ_PRI5	5
#define NVIC_IRQ_PRI6	6
#define NVIC_IRQ_PRI7	7
#define NVIC_IRQ_PRI8	8
#define NVIC_IRQ_PRI9	9
#define NVIC_IRQ_PRI10	10
#define NVIC_IRQ_PRI11	11
#define NVIC_IRQ_PRI12	12
#define NVIC_IRQ_PRI13	13
#define NVIC_IRQ_PRI14	14
#define NVIC_IRQ_PRI15	15


/*
 * Generic Macros
 */
#define ENABLE			1
#define DISABLE			0
#define HIGH			ENABLE
#define LOW             DISABLE
#define SET				ENABLE
#define RESET			DISABLE
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET
#define FLAG_RESET		RESET
#define FLAG_SET		SET

/*******************************************************************************************************************
 * 								Bit Positions of SPI Peripheral
 *******************************************************************************************************************/

/*
 *  SPI_CR1 Bit Position Definitions
 */
#define SPI_CR1_CPHA		0
#define SPI_CR1_CPOL		1
#define SPI_CR1_MSTR		2
#define SPI_CR1_BR			3
#define SPI_CR1_SPE			6
#define SPI_CR1_LSBFIRST	7
#define SPI_CR1_SSI			8
#define SPI_CR1_SSM			9
#define SPI_CR1_RXONLY		10
#define SPI_CR1_DFF			11
#define SPI_CR1_CRCNEXT		12
#define SPI_CR1_CRCEN		13
#define SPI_CR1_BIDIOE		14
#define SPI_CR1_BIDIMODE	15


/*
 *  SPI_CR2 Bit Position Definitions
 */
#define SPI_CR2_RXDMAEN		0
#define SPI_CR2_TXDMAEN		1
#define SPI_CR2_SSOE		2
// 3 bit is Reserved
#define SPI_CR2_FRF			4
#define SPI_CR2_ERRIE		5
#define SPI_CR2_RXNEIE		6
#define SPI_CR2_TXEIE		7


/*
 *  SPI_SR Bit Position Definitions
 */
#define SPI_SR_RXNE			0
#define SPI_SR_TXE			1
#define SPI_SR_CHSIDE		2
#define SPI_SR_UDR			3
#define SPI_SR_CRCERR		4
#define SPI_SR_MODF			5
#define SPI_SR_OVR			6
#define SPI_SR_BSY			7
#define SPI_SR_FRE			8


/*******************************************************************************************************************
 * 								Bit Positions of I2C Peripheral
 *******************************************************************************************************************/
/*
 *  I2C_CR1 Bit Position Definitions
 */
#define I2C_CR1_PE				0
#define I2C_CR1_SMBUS			1
#define I2C_CR1_SMBTYPE			3
#define I2C_CR1_ENARP			4
#define I2C_CR1_ENPEC			5
#define I2C_CR1_ENGC			6
#define I2C_CR1_NOSTRETCH		7
#define I2C_CR1_START			8
#define I2C_CR1_STOP			9
#define I2C_CR1_ACK				10
#define I2C_CR1_POS				11
#define I2C_CR1_PEC				12
#define I2C_CR1_ALERT			13
#define I2C_CR1_SWRST			15


/*
 * Bit position definitions I2C_CR2
 */
#define I2C_CR2_FREQ				 	0
#define I2C_CR2_ITERREN				 	8
#define I2C_CR2_ITEVTEN				 	9
#define I2C_CR2_ITBUFEN 			    10


/*
 * Bit position definitions I2C_OAR1
 */
#define I2C_OAR1_ADD0    				 0
#define I2C_OAR1_ADD71 				 	 1
#define I2C_OAR1_ADD98  			 	 8
#define I2C_OAR1_ADDMODE   			 	15


/*
 * Bit position definitions I2C_SR1
 */

#define I2C_SR1_SB 					 	0
#define I2C_SR1_ADDR 				 	1
#define I2C_SR1_BTF 					2
#define I2C_SR1_ADD10 					3
#define I2C_SR1_STOPF 					4
#define I2C_SR1_RXNE 					6
#define I2C_SR1_TXE 					7
#define I2C_SR1_BERR 					8
#define I2C_SR1_ARLO 					9
#define I2C_SR1_AF 					 	10
#define I2C_SR1_OVR 					11
#define I2C_SR1_TIMEOUT 				14


/*
 * Bit position definitions I2C_SR2
 */
#define I2C_SR2_MSL						0
#define I2C_SR2_BUSY 					1
#define I2C_SR2_TRA 					2
#define I2C_SR2_GENCALL 				4
#define I2C_SR2_DUALF 					7


/*
 * Bit position definitions I2C_CCR
 */
#define I2C_CCR_CCR 					 0
#define I2C_CCR_DUTY 					14
#define I2C_CCR_FS  				 	15


/******************************************************************************************
 *							Bit position definitions of USART peripheral
 ******************************************************************************************/

/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_SBK					0
#define USART_CR1_RWU 					1
#define USART_CR1_RE  					2
#define USART_CR1_TE 					3
#define USART_CR1_IDLEIE 				4
#define USART_CR1_RXNEIE  				5
#define USART_CR1_TCIE					6
#define USART_CR1_TXEIE					7
#define USART_CR1_PEIE 					8
#define USART_CR1_PS 					9
#define USART_CR1_PCE 					10
#define USART_CR1_WAKE  				11
#define USART_CR1_M 					12
#define USART_CR1_UE 					13
#define USART_CR1_OVER8  				15


/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_ADD   				0
#define USART_CR2_LBDL   				5
#define USART_CR2_LBDIE  				6
#define USART_CR2_LBCL   				8
#define USART_CR2_CPHA   				9
#define USART_CR2_CPOL   				10
#define USART_CR2_STOP   				12
#define USART_CR2_LINEN   				14


/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE   				0
#define USART_CR3_IREN   				1
#define USART_CR3_IRLP  				2
#define USART_CR3_HDSEL   				3
#define USART_CR3_NACK   				4
#define USART_CR3_SCEN   				5
#define USART_CR3_DMAR  				6
#define USART_CR3_DMAT   				7
#define USART_CR3_RTSE   				8
#define USART_CR3_CTSE   				9
#define USART_CR3_CTSIE   				10
#define USART_CR3_ONEBIT   				11


/*
 * Bit position definitions USART_SR
 */

#define USART_SR_PE        				0
#define USART_SR_FE        				1
#define USART_SR_NE        				2
#define USART_SR_ORE       				3
#define USART_SR_IDLE       			4
#define USART_SR_RXNE        			5
#define USART_SR_TC        				6
#define USART_SR_TXE        			7
#define USART_SR_LBD        			8
#define USART_SR_CTS        			9




#include "stm32f407xx_gpio_driver.h"

#include "stm32f407xx_spi_driver.h"

#include "stm32f407xx_i2c_driver.h"

#include "stm32f407xx_usart_driver.h"

#include "stm32f407xx_rcc_driver.h"



#endif /* INC_STM32F407XX_H_ */
