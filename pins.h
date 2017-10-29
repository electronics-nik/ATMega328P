/*
 * pins.h
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 27.06.2017 22:25:01
 *  Author: Nikolay  
 */ 


#ifndef PINS_H_
#define PINS_H_

#define HIGH_LEVEL		1
#define LOW_LEVEL		0

#define PIN_READ(port, pin)						(port & ( 1 << pin ))
#define PIN_WRITE(port, pin, value)				((value == LOW_LEVEL) ? (port &= ~(1 << pin)) : (port |= (1 << pin)))
#define PIN_MODE_OUTPUT(ddr, pin)				(ddr |= ( 1 << pin ))
#define PIN_MODE_INPUT(port, ddr, pin)			(ddr &= ~( 1 << pin )); (port &= ~( 1 << pin ))
#define PIN_MODE_INPUT_PULLUP(port, ddr, pin)	(ddr &= ~( 1 << pin )); (port |= ( 1 << pin ))

#endif /* PINS_H_ */