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

#define pin_read(in_port, pin)						(in_port & ( 1 << pin ))
#define pin_write(out_port, pin, value)				((value == LOW_LEVEL) ? (out_port &= ~(1 << pin)) : (out_port |= (1 << pin)))
#define pin_mode_output(ddr, pin)					(ddr |= ( 1 << pin ))
#define pin_mode_input(out_port, ddr, pin)			(ddr &= ~( 1 << pin )); (out_port &= ~( 1 << pin ))
#define pin_mode_input_pullup(out_port, ddr, pin)	(ddr &= ~( 1 << pin )); (out_port |= ( 1 << pin ))

#endif /* PINS_H_ */