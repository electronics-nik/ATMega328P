#ifndef SYS_LED_H_ 
#define SYS_LED_H_

#include "pins.h"

#define led_init	PIN_MODE_OUTPUT(DDRB, PB5)
#define led_on		PIN_WRITE(PORTB, PB5, HIGH_LEVEL)					
#define led_off		PIN_WRITE(PORTB, PB5, LOW_LEVEL)

#endif /* SYS_LED_H_ */