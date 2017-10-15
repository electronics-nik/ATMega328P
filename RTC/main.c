/*
 * RTC.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * RTC: DS1307
 *
 * Created: 15.10.2017 21:43:19
 * Author : Nikolay
 */ 

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

	_delay_ms(1000);

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
	init(false);
	char buffer[256] = {};
	sei();

    while (true) 
    {
		cli();
		
		if(flag)
		{
			sprintf(buffer, "%02d.%02d.%02d   %02d:%02d", t.tm_mday, t.tm_mon, t.tm_year, t.tm_hour, t.tm_min);
		}
		else
		{
			sprintf(buffer, "%02d.%02d.%02d   %02d %02d", t.tm_mday, t.tm_mon, t.tm_year, t.tm_hour, t.tm_min);
		}

		tx_uart_puts(buffer);
		tx_uart_puts("\r\n");

		sei();

		_delay_ms(60);
    }

	return 0;
}

