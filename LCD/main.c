/*
 * LCD.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * LCD: 16x2 HD44780 (или аналог)
 *
 * Created: 15.10.2017 22:09:20
 * Author : Nikolay
 */ 

#include "lcd.h"
#include "rtc.h"
#include "tx_uart.h"

void init(bool is_set_time)
{
	EICRA &= ~( ( 1 << ISC11 ) | ( 1 << ISC10 ) | ( 1 << ISC01 ) | ( 1 << ISC00 ) );
	EICRA |= ( ( 1 << ISC01 ) | ( 1 << ISC00 ) );
	
	EIMSK = 0;
	EIMSK |= ( 1 << INT0 );

	tx_uart_init();
	tx_uart_puts("TX UART inited\n");

	rtc_init();
	tx_uart_puts("RTC inited\n");

	lcd_init();
	tx_uart_puts("LCD inited\n");

	lcd_clear(LIGHT_ON);

	lcd_print_line1("Home  Controller", LIGHT_ON);
	lcd_print_line2("     v 1.0", LIGHT_ON);

	_delay_ms(3000);

	if (is_set_time)
	{
		struct tm t;

		t.tm_hour = 23;
		t.tm_min = 45;
		t.tm_sec = 0;

		t.tm_mday = 25;
		t.tm_mon = 9;
		t.tm_year = 17;

		rtc_setdate(&t);
	}
}

struct tm t;
bool flag = true;

ISR(INT0_vect)
{
	rtc_getdate(&t);
	flag = !flag;
}

int main(void)
{
    cli();
	char lcd_buffer[16] = {};

	init(false);

	lcd_print_line1("Current time:   ", LIGHT_ON);

	sei();


    while (true) 
    {
		cli();
		
		if(flag)
		{
			sprintf(lcd_buffer, "%02d.%02d.%02d   %02d:%02d", t.tm_mday, t.tm_mon, t.tm_year, t.tm_hour, t.tm_min);
		}
		else
		{
			sprintf(lcd_buffer, "%02d.%02d.%02d   %02d %02d", t.tm_mday, t.tm_mon, t.tm_year, t.tm_hour, t.tm_min);
		}

		lcd_print_line2(lcd_buffer, LIGHT_ON);
		
		sei();

		_delay_ms(60);
    }
}

