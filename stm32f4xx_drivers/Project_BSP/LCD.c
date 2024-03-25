/*
 * LCD.c
 *
 *  Created on: Mar 3, 2024
 *      Author: mahen
 */
#include "lcd.h"

static void write_4_bits(uint8_t value);
static void lcd_enable(void);
static void mdelay(uint32_t cnt);
static void udelay(uint32_t cnt);

/************************************Helper Functions ********************************************************/

static void mdelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1000); i++);
}


static void udelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1); i++);
}


static void write_4_bits(uint8_t value)
{
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D4, ((value >> 0) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D5, ((value >> 1) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D6, ((value >> 2) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D7, ((value >> 3) & 0x1) );

	lcd_enable();
}


static void lcd_enable(void)
{
	//Setting enable
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);

	//waiting for 10 microseconds
	udelay(10);

	//Resetting enable
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);

	//waiting for 100 microseconds (execution time > 37 micro seconds)
	udelay(100);
}


void lcd_send_command(uint8_t cmd)
{
	// RS=0
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	//R/nW = 0
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	//Sending higher nibble
	write_4_bits(cmd >> 4);

	//Sending lower nibble
	write_4_bits(cmd & 0x0F);
}


void lcd_print_char(uint8_t data)
{
	// RS=1
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);

	//R/nW = 0
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	//Sending higher nibble
	write_4_bits(data >> 4);

	//Sending lower nibble
	write_4_bits(data & 0x0F);
}


void lcd_display_clear(void)
{
	//Display clear command
	lcd_send_command(LCD_CMD_DIS_CLEAR);

	// display clear command execution wait time is around 2ms
	mdelay(2);
}


void lcd_print_string(char *message)
{
      do
      {
          lcd_print_char((uint8_t)*message++);
      }
      while (*message != '\0');
}


void lcd_display_return_home(void)
{
	//Display return home command
	lcd_send_command(LCD_CMD_DIS_RETURN_HOME);

	// display return home command execution wait time is around 2ms
	mdelay(2);
}


void lcd_set_cursor(uint8_t row, uint8_t column)
{
  column--;
  switch (row)
  {
    case 1:
      // Set cursor to 1st row address and add index
      lcd_send_command((column |= 0x80));
      break;
    case 2:
      // Set cursor to 2nd row address and add index
        lcd_send_command((column |= 0xC0));
      break;
    default:
      break;
  }
}

/********************************************************************************************
 * @fn      		  - lcd_init
 *
 * @brief             - This function Initializes the LCD
 *
 * @param[in]         - none
 *
 * @return            - none
 *
 * @Note              - none
 *
 *********************************************************************************************/
void lcd_init(void)
{

	//1. Configure the gpio pins which are used for lcd connections

	GPIO_Handle_t lcd_signal;

	//RS
	lcd_signal.pGPIOx = LCD_GPIO_PORT;
	lcd_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RS;
	lcd_signal.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	lcd_signal.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_SPEED_FAST;

	GPIO_Init(&lcd_signal);

	//RW
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RW;
	GPIO_Init(&lcd_signal);

	//EN
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_EN;
	GPIO_Init(&lcd_signal);

	//D4
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D4;
	GPIO_Init(&lcd_signal);

	//D5
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D5;
	GPIO_Init(&lcd_signal);

	//D6
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D6;
	GPIO_Init(&lcd_signal);

	//D7
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D7;
	GPIO_Init(&lcd_signal);

	// Logic 0 -> Initially for all the pins
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);



	//2. Do the LCD initialization

	// waiting foe 40 milliseconds
	mdelay(40);

	//RS is pulled to LOW(0)
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	// RnW is pulled to LOW(0)
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	//Writing as per flow chart 4 bits of data/command on to D4,D5,D6,D7 lines */
	write_4_bits(0x3);

	//waiting for more than 4.1 milliseconds
	mdelay(5);

	//Writing as per flow chart 4 bits of data/command on to D4,D5,D6,D7 lines */
	write_4_bits(0x3);

	//waiting for more than 100 microseconds
	udelay(150);

	//Writing as per flow chart 4 bits of data/command on to D4,D5,D6,D7 lines */
	write_4_bits(0x3);

	//Writing as per flow chart 4 bits of data/command on to D4,D5,D6,D7 lines */
	write_4_bits(0x2);

	//1. function set command
	lcd_send_command(LCD_CMD_4DL_2N_5X8F);

	//2. disply ON and cursor ON
	lcd_send_command(LCD_CMD_DON_CURON);

	//3. display Clear
	lcd_display_clear();

	//4. entry mode set
	lcd_send_command(LCD_CMD_INCADD);

}











