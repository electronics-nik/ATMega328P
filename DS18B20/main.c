/*
 * DS18B20.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * Temperature sensor: DS18B20
 *
 * Created: 20.10.2017 21:18:28
 * Author : Nikolay
 */
  

#include "../pins.h"
#include "i2c.h"
#include "lcd.h"
#include "rtc.h"
#include "tx_uart.h"
#include "ds18b20.h"

struct tm t;
bool flag = true;
bool _sign = false;

uint8_t mantis = 0;
uint8_t ex = 0;

ISR(INT0_vect)
{
	uint8_t ROM[9] = {};
	static uint8_t counter = 0;

	rtc_getdate(&t);
	flag = !flag;

	StartConvertTempForAllSensor();
	if (GetStatusDevice1(ROM))
	{
		GetTemperature(ROM[REG_LSB], ROM[REG_MSB], &_sign, &mantis, &ex);
	}

	if (counter == 5)
	{
		counter = 0;
	}
	else 
	{
		counter ++;
	}
}

void init(bool is_set_time)
{
	EICRA &= ~( ( 1 << ISC11 ) | ( 1 << ISC10 ) | ( 1 << ISC01 ) | ( 1 << ISC00 ) );
	EICRA |= ( ( 1 << ISC01 ) | ( 1 << ISC00 ) );
	//
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
		t.tm_min = 40;
		t.tm_sec = 0;

		t.tm_mday = 23;
		t.tm_mon = 10;
		t.tm_year = 17;

		rtc_setdate(&t);
	}
}

int main(void)
{
	cli();
	char lcd_buffer[16] = {};

	init(false);

	lcd_print_line1("                ", LIGHT_ON);

	PIN_MODE_OUTPUT(DDRB, PB5);

	sei();

	while (true)
	{	
		cli();

		sprintf(lcd_buffer, "Temp: %c%2d.%01d", (_sign == 0 ? '+' : '-'), mantis, ex);
		lcd_print_line1(lcd_buffer, LIGHT_ON);
			
		if(flag)
		{
			sprintf(lcd_buffer, "%02d.%02d.%02d   %02d:%02d", t.tm_mday, t.tm_mon, t.tm_year, t.tm_hour, t.tm_min);
			PIN_WRITE(PORTB, PB5, LOW_LEVEL);
		}
		else
		{
			sprintf(lcd_buffer, "%02d.%02d.%02d   %02d %02d", t.tm_mday, t.tm_mon, t.tm_year, t.tm_hour, t.tm_min);
			PIN_WRITE(PORTB, PB5, HIGH_LEVEL);
		}

		lcd_print_line2(lcd_buffer, LIGHT_ON);
		
		sei();
	}
}