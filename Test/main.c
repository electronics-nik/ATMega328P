/*
 * Test.c
 *
 * Created: 22.10.2017 0:09:45
 * Author : Nikolay
 */ 

#include "../constants.h"
#include "../pins.h"
#include <avr/wdt.h>


int main(void)
{
	PIN_MODE_OUTPUT(DDRB, PB5);
	PIN_WRITE(PORTB, PB5, LOW_LEVEL);
    /* Replace with your application code */
	wdt_enable(WDTO_2S);
	PIN_WRITE(PORTB, PB5, HIGH_LEVEL);
	_delay_ms(2500);

    while (1) 
    {
		wdt_reset();
    }
}

