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

#define PIN_WRITE(port, pin, value)				((value == LOW_LEVEL) ? (PORT##port &= ~(1 << pin)) : (PORT##port |= (1 << pin)))
#define PIN_READ(port,pin)						((PIN##port & (1 << pin)) ? HIGH_LEVEL : LOW_LEVEL)
#define PIN_MODE_OUTPUT(port, pin)				DDR##port |= (1 << pin)
#define PIN_MODE_INPUT(port, pin)				DDR##port &= ~(1 << pin); PIN_WRITE(port, pin, LOW_LEVEL)
#define PIN_MODE_INPUT_PULLUP(port, pin)		DDR##port &= ~(1 << pin); PIN_WRITE(port, pin, HIGH_LEVEL)

#endif /* PINS_H_ */