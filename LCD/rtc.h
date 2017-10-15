/*
 * rtc.h
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * RTC: DS1307
 *
 * Created: 15.10.2017 21:44:41
 *  Author: Nikolay
 */ 


#ifndef RTC_H_
#define RTC_H_

#include "../constants.h"
#include "i2c.h"

// Memory (24C32N) Address
#define A24C32N_ADDR		0xA0
#define A24C32N_WRITE		A24C32N_ADDR | I2C_WRITE
#define A24C32N_READ		A24C32N_ADDR | I2C_READ

// RTC Address
#define DS1307Z_ADDR		0xD0
#define DS1307Z_WRITE		DS1307Z_ADDR | I2C_WRITE
#define DS1307Z_READ		DS1307Z_ADDR | I2C_READ

// RTC registers
#define RS0					0x00
#define RS1					0x01
#define SQWE				0x04
#define OUT					0x07

#define SQWE_OUT_REG		0x07

// RTC
extern void rtc_init( void );
extern uint8_t rtc_setdate( struct tm* cur_time );
extern void rtc_getdate( struct tm* cur_time );

#endif /* RTC_H_ */