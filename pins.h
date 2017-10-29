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

#define PIN_READ(in_port, pin)						(in_port & ( 1 << pin ))
#define PIN_WRITE(out_port, pin, value)				((value == LOW_LEVEL) ? (out_port &= ~(1 << pin)) : (out_port |= (1 << pin)))
#define PIN_MODE_OUTPUT(ddr, pin)					(ddr |= ( 1 << pin ))
#define PIN_MODE_INPUT(out_port, ddr, pin)			(ddr &= ~( 1 << pin )); (out_port &= ~( 1 << pin ))
#define PIN_MODE_INPUT_PULLUP(out_port, ddr, pin)	(ddr &= ~( 1 << pin )); (out_port |= ( 1 << pin ))

#endif /* PINS_H_ */