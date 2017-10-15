/*
 * constants.h
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 15.10.2017 13:46:54
 *  Author: Nikolay
 */

#define F_CPU			16000000UL
#define F_SCL			100000UL
#define BAUD_RATE		9600UL
#define SOFT_BAUD_RATE	9600UL

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <time.h>
#include <stdbool.h>
#include <avr/interrupt.h>



