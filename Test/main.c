/*
 * Test.c
 *
 * Created: 22.10.2017 0:09:45
 * Author : Nikolay
 */ 

#include "../constants.h"
#include "../sys_led.h"
#include <avr/wdt.h>


int main(void)
{
	led_init;
	led_off;
    /* Replace with your application code */
	wdt_enable(WDTO_2S);
	led_on;
	_delay_ms(2500);

    while (1) 
    {
		wdt_reset();
    }
}

