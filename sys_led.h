#ifndef SYS_LED_H_ 
#define SYS_LED_H_

#include "pins.h"

#define led_init	pin_mode_output(DDRB, PB5)
#define led_on		pin_write(PORTB, PB5, HIGH_LEVEL)					
#define led_off		pin_write(PORTB, PB5, LOW_LEVEL)

#endif /* SYS_LED_H_ */