/*
 * DS1307.c
 *
 *  Created on: Mar 3, 2024
 *      Author: mahen
 */

#include "DS1307.h"

#include<stdint.h>

#include<string.h>

I2C_Handle_t g_ds1307I2cHandle;

static void ds1307_i2c_pin_config(void);
static void ds1307_i2c_config(void);
static uint8_t ds1307_read(uint8_t reg_addr);
static void ds1307_write(uint8_t value,uint8_t reg_addr);
static uint8_t bcd_to_binary(uint8_t value);
static uint8_t binary_to_bcd(uint8_t value);



/************************************Helper Functions ********************************************************/

static void ds1307_i2c_pin_config(void)
{
	GPIO_Handle_t i2c_sda,i2c_scl;

	memset(&i2c_sda,0,sizeof(i2c_sda));
	memset(&i2c_scl,0,sizeof(i2c_scl));

	/*
	 * I2C1_SCL ==> PB6
	 * I2C1_SDA ==> PB7
	 */

	i2c_sda.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;

	GPIO_Init(&i2c_sda);


	i2c_scl.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;

	GPIO_Init(&i2c_scl);

}


static void ds1307_i2c_config(void)
{
	g_ds1307I2cHandle.pI2Cx = DS1307_I2C;
	g_ds1307I2cHandle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	g_ds1307I2cHandle.I2C_Config.I2C_SCLSpeed = DS1307_I2C_SPEED;

	I2C_Init(&g_ds1307I2cHandle);
}


static void ds1307_write(uint8_t value, uint8_t reg_addr)
{
	uint8_t tx[2];
	tx[0] = reg_addr;
	tx[1] = value;

	I2C_MasterSendData(&g_ds1307I2cHandle, tx, 2, DS1307_I2C_ADDRESS, 0);
}


static uint8_t ds1307_read(uint8_t reg_addr)
{
	uint8_t data;

    I2C_MasterSendData(&g_ds1307I2cHandle, &reg_addr, 1, DS1307_I2C_ADDRESS, 0);
    I2C_MasterReceiveData(&g_ds1307I2cHandle, &data, 1, DS1307_I2C_ADDRESS, 0);

    return data;
}


static uint8_t binary_to_bcd(uint8_t value)
{
	uint8_t m , n;
	uint8_t bcd;

	bcd = value;
	if(value >= 10)
	{
		m = value /10; // Tenths place
		n = value % 10; // Units place
		bcd = (m << 4) | n ;
	}

	return bcd;
}


static uint8_t bcd_to_binary(uint8_t value)
{
	uint8_t m , n;
	m = (uint8_t) ((value >> 4 ) * 10);
	n =  value & (uint8_t)0x0F;
	return (m+n);
}


/********************************************************************************************
 * @fn      		  - ds1307_init
 *
 * @brief             - This function Initializes the DS1307 I2C Communication
 *
 * @param[in]         - none
 *
 * @return            - 1 or 0
 *
 * @Note              - returns 1 : CH = 1 ; init failed
						returns 0 : CH = 0 ; init success

 *********************************************************************************************/

uint8_t ds1307_init(void)
{

	//1. Initializing the I2C Pins
	ds1307_i2c_pin_config();

	//2. initialize the I2C Peripheral
	ds1307_i2c_config();

	//3. Enabling the I2C Peripheral
	I2C_PeripheralControl(DS1307_I2C, ENABLE);

	//4. Make CH = 0 for resuming the internal clock
	ds1307_write(0x00,DS1307_ADDR_SEC);

	//5. Read back CH bit
	uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC);

	return ((clock_state >> 7 ) & 0x1);

}


/********************************************************************************************
 * @fn      		  - ds1307_set_current_time
 *
 * @brief             - This function sets the current time in DS1307 module
 *
 * @param[in]         - Pointer to time structure that contains all the current time information
 *
 * @return            - None
 *
 * @Note              - None

 *********************************************************************************************/

void ds1307_set_current_time(RTC_time_t *rtc_time)
{
	uint8_t seconds, hours;

	//seconds
	seconds = binary_to_bcd(rtc_time->seconds);
	seconds &= ~( 1 << 7);
	ds1307_write(seconds, DS1307_ADDR_SEC);

	//Minutes
	ds1307_write(binary_to_bcd(rtc_time->minutes), DS1307_ADDR_MIN);

	//Hours
	hours = binary_to_bcd(rtc_time->hours);

	if(rtc_time->time_format == TIME_FORMAT_24HRS)
	{
		// 24-hour format
		hours &= ~(1 << 6);
	}else
	{
		//12-hour format
		hours |= (1 << 6);
		hours = (rtc_time->time_format  == TIME_FORMAT_12HRS_PM) ? hours | ( 1 << 5) :  hours & ~( 1 << 5) ;
	}
	ds1307_write(hours,DS1307_ADDR_HRS);
}


/********************************************************************************************
 * @fn      		  - ds1307_set_current_date
 *
 * @brief             - This function sets the current date in DS1307 module
 *
 * @param[in]         - Pointer to date structure that contains all the current date information
 *
 * @return            - None
 *
 * @Note              - None

 *********************************************************************************************/
void ds1307_set_current_date(RTC_date_t *rtc_date)
{
	//Day
	ds1307_write(binary_to_bcd(rtc_date->day),DS1307_ADDR_DAY);

	//Date
	ds1307_write(binary_to_bcd(rtc_date->date),DS1307_ADDR_DATE);

	//Month
	ds1307_write(binary_to_bcd(rtc_date->month),DS1307_ADDR_MONTH);

	//Year
	ds1307_write(binary_to_bcd(rtc_date->year),DS1307_ADDR_YEAR);
}



/********************************************************************************************
 * @fn      		  - ds1307_get_current_time
 *
 * @brief             - This function gets the current time in DS1307 module
 *
 * @param[in]         - Pointer to time structure that contains all the current time information
 *
 * @return            - None
 *
 * @Note              - None

 *********************************************************************************************/
void ds1307_get_current_time(RTC_time_t *rtc_time)
{

	uint8_t seconds,hours;

	//Seconds
	seconds = ds1307_read(DS1307_ADDR_SEC);
	seconds &= ~( 1 << 7);
	rtc_time->seconds = bcd_to_binary(seconds);

	//Minutes
	rtc_time->minutes = bcd_to_binary(ds1307_read(DS1307_ADDR_MIN));

	//Hours
	hours = ds1307_read(DS1307_ADDR_HRS);
	if(hours & ( 1 << 6))
	{
		//12 hr format
		rtc_time->time_format =  !((hours & ( 1 << 5)) == 0) ;
		hours &= ~(0x3 << 5);//Clear 6 and 5
	}else
	{
		//24 hr format
		rtc_time->time_format = TIME_FORMAT_24HRS;
	}
	rtc_time->hours = bcd_to_binary(hours);
}


/********************************************************************************************
 * @fn      		  - ds1307_get_current_date
 *
 * @brief             - This function gets the current date in DS1307 module
 *
 * @param[in]         - Pointer to date structure that contains all the current date information
 *
 * @return            - None
 *
 * @Note              - None

 *********************************************************************************************/
void ds1307_get_current_date(RTC_date_t *rtc_date)
{
	//Day
	rtc_date->day =  bcd_to_binary(ds1307_read(DS1307_ADDR_DAY));

	//Date
	rtc_date->date = bcd_to_binary(ds1307_read(DS1307_ADDR_DATE));

	//Month
	rtc_date->month = bcd_to_binary(ds1307_read(DS1307_ADDR_MONTH));

	//Year
	rtc_date->year = bcd_to_binary(ds1307_read(DS1307_ADDR_YEAR));

}


