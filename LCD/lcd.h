/*
 * lcd.h
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * LCD: 16x2 HD44780 (или аналог)
 *
 * Created: 15.10.2017 22:35:12
 *  Author: Nikolay
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "../constants.h"
#include "i2c.h"

#define PCF857A_ADDR		0x7E
#define PCF857A_WRITE		PCF857A_ADDR | I2C_WRITE

#define RS					0x01
#define RW					0x02
#define EN					0x04
#define LIGHT				0x08
#define D4					0x10
#define D5					0x20
#define D6					0x40
#define D7					0x80

#define LIGHT_ON			1
#define LIGHT_OFF			!LIGHT_ON

#define COMMAND				1
#define DATA				!COMMAND

#define SET_RS(x)			(x |= RS)
#define RESET_RS(x)			(x &= ~RS)
#define SET_RW(x)			(x |= RW)
#define RESET_RW(x)			(x &= ~RW)
#define SET_EN(x)			(x |= EN)
#define RESET_EN(x)			(x &= ~EN)
#define SET_LIGHT(x)		(x |= LIGHT)
#define RESET_LIGHT(x)		(x &= ~LIGHT)
#define SET_D4(x)			(x |= D4)
#define RESET_D4(x)			(x &= ~D4)
#define SET_D5(x)			(x |= D5)
#define RESET_D5(x)			(x &= ~D5)
#define SET_D6(x)			(x |= D6)
#define RESET_D6(x)			(x &= ~D6)
#define SET_D7(x)			(x |= D7)
#define RESET_D7(x)			(x &= ~D7)

// command
#define CLEAR				0x01

#define LINE_1		0x80
#define	LINE_2		0xC0

uint8_t lcd_init(void);
void lcd_clear(uint8_t light_is_on);
void lcd_light(uint8_t on_off);
void lcd_print_line1(char* str, uint8_t light_is_on);
void lcd_print_line2(char* str, uint8_t light_is_on);



#endif /* LCD_H_ */