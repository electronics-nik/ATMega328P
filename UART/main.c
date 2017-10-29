/*
 * UART.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 16.10.2017 2:48:46
 * Author : Nikolay
 */ 

#include "uart.h"
#include "soft_uart.h"

void init()
{
	uart_init(UART_BAUD_SELECT(BAUD_RATE, F_CPU));
	soft_uart_init();
	soft_uart_turn_rx_on();

	sei();
}

int main(void)
{
	char c;
	init();

	_delay_ms( 500 );

	soft_uart_puts("Test SOFT UART\n");

	uart_puts("Test addon\n");
	uart_put_longint( 2000356435 );
	uart_putc( '\n' );
	uart_put_ulongint( 4235634566 );
	uart_putc( '\n' );
	uart_put_int( -567 );
	uart_putc( '\n' );
	uart_puthex_nibble( 12 );
	uart_putc( '\n' );
	uart_puthex_byte( 25 );
	uart_putc( '\n' );
	uart_puthex_long( 45546 );
	uart_putc( '\n' );
	uart_putbin_byte( 123 );
	uart_putc( '\n' );

	uart_puts("Test UART\n");


	
	while( true )
	{
		while ( uart_available() )
		{
			c = uart_getc();
			soft_uart_putchar( c );
		}
		
		while ( soft_uart_kbhit() ) 
		{
			c = soft_uart_getchar();
			uart_putc( c );
		}
	}
}